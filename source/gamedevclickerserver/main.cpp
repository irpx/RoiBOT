#include "stdafx.h"
#include "network\flashserver\FlashServer.h"
#include "gamedevclicker\GameDevClickerMMO.h"
#include "gamedevclicker\Player.h"
#include "util\StringUtils.h"
#include "gamedevclicker\GameMessage.h"
#include "gamedevclicker\GameChatMessage.h"
#include "gamedevclicker\PlayerMoveMessage.h"
#include "gamedevclicker\PlayerActionMessage.h"
#include "gamedevclicker\Entity.h"
#include "util/debug.h"

//#include <boost\system\config.hpp>


#include <thread>
#include <mutex>
#include <map>
#include <functional>
#include <string>

using namespace StringUtils;

#define MULTI_THREAD

#ifdef MULTI_THREAD
std::mutex gameLock;
#else
struct dummyLock{ void lock(){}; void unlock(){}; };
dummyLock gameLock;
#endif

const char messageDelim = '|';
GameDevClickerMMO game;
FlashServer server(34225);

class GameToJsonAdapter
{
public:
	std::string getEntityJsonData(std::shared_ptr<Entity> entity)
	{
		std::stringstream output;
		output << "{";
		output << "\"name\":\"" << entity->getName() << "\",";
		output << "\"position\":[" << entity->x << "," << entity->y << "],";
		output << "\"type\":" << static_cast<int>(entity->getType());

		auto& properties = entity->getProperties();
		if (properties.size() > 0)
		{
			output << ", \"properties\":[";
			unsigned idx = 0;
			for (auto property : properties)
			{
				std::string val = property.second->getValue();
				bool num = property.second->isNumber();

				output << "{\"" << property.first << "\":";
				if (num)
				{
					output << property.second->getValue() << "}";
				}
				else
				{
					output << "\"" << property.second->getValue() << "\"}";
				}
				if (idx < properties.size() - 1)
				{
					output << ", ";
				}
				idx++;
			}
			output << "]";
		}

		output << "}";
		return output.str();
	}

	std::string getMembersJsonData(GameDevClickerMMO& game)
	{
		std::stringstream output;
		output << "{\"members\":[";
		auto players = game.getPlayers();
		for (size_t i = 0; i < players.size(); i++)
		{
			std::shared_ptr<Player> player = players[i];
			output << getEntityJsonData(player);

			if (i < players.size() - 1)
			{
				output << ",";
			}
		}
		output << "]}";
		return output.str();
	}
	std::string getStatsJsonData(std::shared_ptr<Player> player)
	{
		std::stringstream output;
		output << "{\"stats\":[";

		auto properties = player->getProperties();
		unsigned idx = 0;
		for (auto property : properties)
		{
			std::string val = property.second->getValue();
			bool num = property.second->isNumber();

			output << "{";
			output << "\"displayName\":\"" << property.first << ": " << property.second->getValue() << "\", ";
			output << "\"description\":\"TODO\"";
			output << "}";

			if (idx < properties.size() - 1)
			{
				output << ", ";
			}
			idx++;
		}

		output << "]}";
		return output.str();
	}

	std::string getTasksJsonData(std::shared_ptr<Player> player)
	{
		std::stringstream output;
		output << "{\"tasks\":[";

		auto& tasks = player->tasks;
		for (size_t i = 0; i < tasks.size(); i++)
		{
			auto task = tasks[i];
			output << "{";
			output << "\"id\":" << task->id << ", ";
			output << "\"displayName\":\"" << task->name << "\", ";
			output << "\"description\":\"" << task->getDescription() << "\", ";
			output << "\"onClick\":\"taskInfo{" << task->id << "}\"";
			output << "}";

			if (i < tasks.size() - 1)
			{
				output << ", ";
			}
		}

		output << "]}";
		return output.str();
	}

	std::string getEntitiesJsonData(Entity* entity)
	{
		std::stringstream output;
		output << "{\"entities\":[";
		if (entity->getType() == Entity::entityType::ROOM)
		{
			Room* container = dynamic_cast<Room*>(entity);

			auto& entities = container->getEntities();
			for (size_t i = 0; i < entities.size(); i++)
			{
				std::shared_ptr<Entity> entity = entities[i];

				output << getEntityJsonData(entity);

				if (i < entities.size() - 1)
				{
					output << ", ";
				}
			}
		}
		output << "]}";
		return output.str();
	};

	std::string getPlayerActionJsonData(std::shared_ptr<PlayerActionMessage> msg)
	{
		std::stringstream output;
		output << "{\"player\":\"" << msg->playerName << "\", \"action\":\"" << msg->actionName << "\", \"target\":\"" << msg->targetName << "\"}";
		return output.str();
	};

	std::string getDungeonJsonData(StudioDungeon* dungeon)
	{
		std::stringstream gameInfo;
		gameInfo << "{";

		auto rooms = dungeon->getRooms();

		gameInfo << "\"name\":\"" << dungeon->getName() << "\", ";
		gameInfo << "\"rooms\":[";
		for (size_t i = 0; i < rooms.size(); i++)
		{
			auto room = rooms[i];
			auto size = room->getSize();
			gameInfo << "{";
			gameInfo << "\"name\":\"" << room->getName() << "\", ";
			gameInfo << "\"width\":" << std::get<0>(size) << ", ";
			gameInfo << "\"height\":" << std::get<1>(size) << ", ";
			gameInfo << "\"x\":" << room->x << ", ";
			gameInfo << "\"y\":" << room->x;// << ", ";
			gameInfo << "}";

			if (i < rooms.size() - 1)
			{
				gameInfo << ", ";
			}
		}

		gameInfo << "]";
		gameInfo << "}";
		return gameInfo.str();
	};
};

class JsonToGameAdapter
{
public:
	std::shared_ptr<GameChatMessage> getChatMessage(std::string& data, int playerID)
	{
		std::string name = game.getPlayer(playerID)->getName().c_str();
		std::string msg = trim(data);
		auto pair = splitToPair(msg, ':');
		msg = pair.second;
		//Strip "quotes"
		msg.erase(0, 1);
		msg.erase(msg.size() - 1, 1);

		return std::make_shared<GameChatMessage>(msg, name);
	};
};

std::map<std::string, std::function<std::string(std::string)>> gameBindings;
GameToJsonAdapter jsonAdapter;
JsonToGameAdapter clientAdapter;

std::string playerChat(std::string data)
{
	gameLock.lock();
	game.addClientInput(clientAdapter.getChatMessage(data, server.getActiveConnection()->getClientID()));
	gameLock.unlock();
	return "";
};

std::string requestGame(std::string data)
{
	gameLock.lock();
	auto dungeon = game.getDungeon();
	std::string output = "OnConnect" + jsonAdapter.getDungeonJsonData(dungeon) + messageDelim;
	gameLock.unlock();
	return output;
}

std::string playerJoin(std::string data)
{
	auto pair = splitToPair(data, ':');
	std::string name = pair.second;
	//Strip "quotes"
	name.erase(0, 1);
	name.erase(name.size() - 1, 1);

	name = trim(name);

	//Link with the connection
	auto connection = server.getActiveConnection();
	int playerID = connection->getClientID();
	
	gameLock.lock();
	game.addPlayer(name, playerID);

	//Send contents of the active room
	auto player = game.getPlayer(playerID);

	std::string output;
	std::string members = "UpdateMembers" + jsonAdapter.getMembersJsonData(game) + messageDelim;
	
	output.append("JoinedGame{\"name\":\"" + name + "\",\"id\":" + std::to_string(playerID) + "}" + messageDelim);
	output.append("UpdateEntities" + jsonAdapter.getEntitiesJsonData(player->getParent()) + messageDelim);
	output.append("UpdateTasks" + jsonAdapter.getTasksJsonData(player) + messageDelim);
	output.append("UpdateStats" + jsonAdapter.getStatsJsonData(player) + messageDelim);
	//output.append("UpdateMembers" + jsonAdapter.getMembersJsonData(game) + messageDelim);
	connection->send(output.c_str());

	gameLock.unlock();

	//Send players to all
	server.sendToAll(members);
	
	return "";
};

void onChat(std::shared_ptr<GameChatMessage> chatmsg)
{
	std::stringstream o;
		o << "PlayerChat";
	o << "{\"text\":\"" << chatmsg->m_chatMessage << "\",\"sender\":\"" << chatmsg->m_senderName << "\"}";
	o << "|";
	server.sendToAll(o.str());
};

void onMove(std::shared_ptr<PlayerMoveMessage> move)
{
	std::stringstream o;
	o << "PlayerMove";
	o << "{\"name\":\"" << move->m_name << "\",\"x\":" << move->m_x << ", \"y\":" << move->m_y << "}";
	o << messageDelim;
	server.sendToAll(o.str());
};

void onPlayerAction(std::shared_ptr<PlayerActionMessage> msg)
{
	server.sendToAll("PlayerAction" + jsonAdapter.getPlayerActionJsonData(msg) + messageDelim);
}

std::string playerMove(std::string data)
{
	int playerID = server.getActiveConnection()->getClientID();
	auto pair = splitToPair(data, ',');

	auto first = splitToPair(pair.first, ':');
	auto last = splitToPair(pair.second, ':');

	int x = 0;
	int y = 0;

	auto check = [&x, &y](std::string name, std::string value)
	{
		name.erase(0, 1);
		name.erase(name.size() - 1, 1);
		if (name == "x")
		{
			x = std::stoi(value);
		}
		else if (name == "y")
		{
			y = std::stoi(value);
		}
		else
		{
			//Fuck it
		}
	};

	check(first.first, first.second);
	check(last.first, last.second);

	auto gcm = std::make_shared<PlayerMoveMessage>("", playerID, x, y);
	gameLock.lock();
	game.addClientInput(gcm);
	gameLock.unlock();
	return "";
};

std::string playerCallback(std::string data)
{
	//int taskID = std::stoi(data);
	gameLock.lock();
	printf("GOT DATA[%s]", data.c_str());
	auto tasks = game.getPlayer(server.getActiveConnection()->getClientID())->tasks;
	std::string text;
	for (auto task : tasks)
	{
		text.append(task->getDescription());
	}
	gameLock.unlock();
	return "ShowPopup{\"id\":0, \"spacing\":2, \"components\":[{\"type\":\"componentTypeText\", \"data\":\"Task\"},{\"type\":\"componentTypeText\", \"data\":\"" + text + "\"}]}|";
}

void updatePanel(std::string panelName)
{
	std::stringstream output;
	output << "UpdatePanel{\"name\":\"" << panelName << "\", \"panel\":[";

	//TODO
	//UpdatePanel{ \"name\":\"Test\",\"panel\":[{\"displayName\":\"lol\"}]}|

	output << "]}|";
	//socketInfo->sendQueue.append(output.str());
}

void onDisconnect(int playerID)
{
	std::stringstream data;
	std::string name = "null";
	data << "OnDisconnect{\"name\":\"";
	

	gameLock.lock();
	auto player = game.getPlayer(playerID);
	if (player != nullptr	)
	{
		name = player->getName().c_str();
		game.removePlayer(name, playerID);
	}
	gameLock.unlock();

	data << name;
	data << "\", \"id\":" << playerID << "}" + messageDelim;
	server.sendToAll(data.str());
};

int _tmain(int argc, _TCHAR* argv[])
{
	LOG("Game server init.");
	auto onData = [](std::string message) -> std::string
	{
		std::string messageBuffer;

		messageBuffer.append(message);

		size_t pos = messageBuffer.find(messageDelim);

		if (pos == std::string::npos)
		{
			return "";
		}
		std::string command = messageBuffer.substr(0, pos);
		std::string data = StringUtils::stripBetweenCharacters(command, '{', '}', true);
		messageBuffer.erase(0, pos + 1);

		auto cb = gameBindings.find(command);
		if (cb != gameBindings.end())
		{
			return cb->second(data);
		}
		return "";
	};

	auto onUpdate = [&](void)->void
	{
		gameLock.lock();
#ifndef MULTI_THREAD
		game.update();
#endif

		std::vector<std::shared_ptr<GameMessage>>& messages = game.getClientOutputQueue();

		for (auto message : messages)
		{
			//printf("GAmeMEssaga:%d" , message->m_type);
			switch (message->m_type)
			{
			case GameMessage::gameMessageType::GAME_CHAT:
				onChat(std::dynamic_pointer_cast<GameChatMessage>(message));
				break;
			case GameMessage::gameMessageType::PLAYER_MOVE:
				onMove(std::dynamic_pointer_cast<PlayerMoveMessage>(message));
				break;
			case GameMessage::gameMessageType::PLAYER_ACTION:
				onPlayerAction(std::dynamic_pointer_cast<PlayerActionMessage>(message));
				break;
			default:
				break;
			}
		}
		messages.clear();//Clear all
		gameLock.unlock();
	};

	auto serverUpdate = [&]()
	{
		gameBindings.insert(std::make_pair("requestGame", requestGame));
		gameBindings.insert(std::make_pair("chat", playerChat));
		gameBindings.insert(std::make_pair("move", playerMove));
		gameBindings.insert(std::make_pair("join", playerJoin));
		gameBindings.insert(std::make_pair("callback", playerCallback));

		while (true)
		{
			server.update();
		}
	};

	server.setOnData(onData);
	server.setOnUpdateCallback(onUpdate);
	server.setOnDisconnectCallback(onDisconnect);
	
	game.init();

#ifdef MULTI_THREAD
	std::thread t1(serverUpdate);

	while (game.isRunning())
	{
		onUpdate();
	}
	
	t1.join();
#else
	serverThread();
#endif

	return 0;
}
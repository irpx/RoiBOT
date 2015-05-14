#include "GameDevClickerMMO.h"
#include "Player.h"
#include "GameChatMessage.h"

#include <algorithm>
#include <iostream>
#include <ctime>
#include <cstdlib>

GameDevClickerMMO::GameDevClickerMMO()
	: m_running(true)
{	
}

GameDevClickerMMO::~GameDevClickerMMO()
{
}

void GameDevClickerMMO::init()
{
	m_updateClock = std::chrono::high_resolution_clock::now();
	m_dungeon.getName() = "Epic Spuge Dungeon";
}

void GameDevClickerMMO::addPlayer(std::string name,int id)
{
	gameLog(name + " joined.");
	if (std::find_if(m_players.begin(), m_players.end(), [&name](std::shared_ptr<Player> activePlayer)
	{
		return activePlayer->getName() == name;
	}) != m_players.end())
	{
		log("Player exists already");
		return;
	}

	auto rooms = m_dungeon.getRooms();
	if (rooms.size() == 0)
	{
		log("NO Room for Player!");
		return;
	}

	int selected = rand() % rooms.size();
	auto room = rooms[selected];

	int x = 0;
	int y = 0;
	auto roomSize = room->getSize();

	std::srand(unsigned(std::time(0)));
	auto getRandomPos = [&]()
	{
		x = std::rand() % (std::get<0>(roomSize));
		y = std::rand() % (std::get<1>(roomSize));
		return room->getEntityAtBlock(x, y) == nullptr;
	};

	int count = 0;
	while(!getRandomPos())
	{
		if (count++ > 1000)
		{
			log("Can't find room for player!");
			break;
		}
	}

	std::string o = "PlayerPos:";
	o.append(std::to_string(x));
	o.append(", ");
	o.append(std::to_string(y));
	log(o);
	std::shared_ptr<Player> p = std::make_shared<Player>(name);
	p->setID(id);
	m_players.push_back(p);
	room->addEntity(p);
	p->x = x;
	p->y = y;
}

void GameDevClickerMMO::removePlayer(std::string name,int id)
{
	gameLog(name + " left.");
	auto it = std::find_if(m_players.begin(), m_players.end(), [&name](std::shared_ptr<Player> activePlayer)
	{
		return activePlayer->getName() == name;
	});
	if (it != m_players.end())
	{
		auto player = *it;
		dynamic_cast<EntityContainer*>(player->getParent())->removeEntity(player);
		m_players.erase(it);
	}
	else
	{
		log("Can't find player to remove:" + name);
	}
}

std::vector<std::shared_ptr<Player>> GameDevClickerMMO::getPlayers()
{
	return m_players;
}
void GameDevClickerMMO::update()
{
	auto now = std::chrono::high_resolution_clock::now();
	double dif = std::chrono::duration<double, std::milli>(now - m_updateClock).count();
	double limit = 1000;
	if (dif < limit) return;
	//log("UpdateTime:"+std::to_string(dif));
	//log("Game Update");
	m_updateClock = now;

	//Client Inputs
	for (auto input : m_clientInputQueue)
	{
		input->execute(this);
	}
	m_clientInputQueue.clear();
	//LOGIC UPDATES,
}

void GameDevClickerMMO::log(std::string msg)
{
	std::cout << "[LOG]:" << msg << std::endl;
	//m_gameLog << msg << std::endl;
}
void GameDevClickerMMO::gameLog(std::string msg,std::string sender)
{
	std::cout << "[GAME]:" << msg << std::endl;
	m_gameLog << msg << std::endl;
}

void GameDevClickerMMO::addClientOutput(std::shared_ptr<GameMessage> output)
{
	output->m_state = GameMessage::messageState::CLIENT_OUTPUT_SENT;
	m_clientOutputQueue.push_back(output);
};

void GameDevClickerMMO::addClientInput(std::shared_ptr<GameMessage> input)
{
	input->m_state = GameMessage::messageState::CLIENT_INPUT_SENT;
	m_clientInputQueue.push_back(input);
};

std::shared_ptr<Player> GameDevClickerMMO::getPlayer(std::string name)
{
	for (auto player : m_players)
	{
		if (player->getName() == name)
		{
			return player;
		}
	}
	return nullptr;
}

std::shared_ptr<Player> GameDevClickerMMO::getPlayer(int id)
{
	for (auto player : m_players)
	{
		if (player->getID() == id)
		{
			return player;
		}
	}
	return nullptr;
}
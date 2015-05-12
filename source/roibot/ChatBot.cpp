#include "ChatBot.h"
#include "socketclient/client.hpp"
#include "../util/StringUtils.h"

ChatBot::ChatBot(Client& client, std::string name, std::string uid, std::string channel)
	: m_client(client)
	, m_name(name)
	, m_uid(uid)
	, m_channel(channel)
{
	m_client.addCallback("your_channel", [&](std::string data)
	{
		m_channel = data;
		m_client.emit("chat message", "|/join " + m_channel);
		m_client.emit("chat message", "|/nick " + m_name);
	});
	m_client.addCallback("chat message", [&](std::string data)
	{
		std::vector<std::string> chat = StringUtils::split(data, '|');
		int time = 0;
		int chan = 1;
		int name = 2;
		int msg = 3;

		if (chat[name] == m_name) return;
		
		if (chat[chan] == m_channel)
		{
			std::cout << "[ " << chat[time] << " ] " << chat[name] << ": " << chat[msg] << std::endl;
			onChat(chat[msg]);
		}
		else
		{
			std::cout << std::endl << " - " << chat[chan] << " - [ " << chat[time] << " ] " << chat[name] << ": " << chat[msg] << std::endl;
		}
	});

	m_client.setOnConnect([&]()
	{
		m_client.emit("login", m_uid);
	});
}

ChatBot::~ChatBot()
{
}

void ChatBot::addLocalCommand(std::string name, std::function<void(std::string)> command)
{
	m_localCommands.insert(std::make_pair(name, command));
}

void ChatBot::addChatCommand(std::string name, std::function<void(std::string)> command)
{
	m_chatCommands.insert(std::make_pair(name, command));
}

void ChatBot::command(std::string command)
{
	size_t pos = command.find('/');
	if (pos == 0)
	{
		pos = command.find(' ');
		std::string data = command.substr(pos + 1);
		std::string localCommand = command.substr(1, pos-1);

		auto it = m_localCommands.find(localCommand.c_str());
		if (it != m_localCommands.end())
		{
			auto func = it->second;
			func(data);
		}
		return;
	}
	send(m_channel + "|" + command);
}

void ChatBot::send(std::string message)
{
	m_client.emit("chat message", message);
}

void ChatBot::onChat(std::string chat)
{
	for (auto const& command : m_chatCommands)
	{
		auto func = command.second;
		func(chat);
	}
}

void ChatBot::sendChat(std::string chat)
{
	send(m_channel + "|" + chat);
}

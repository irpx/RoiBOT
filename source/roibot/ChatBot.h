#pragma once

#include <map>
#include <string>
#include <functional>

class Client;
class ChatBot
{
public:
	ChatBot(Client& client,std::string name, std::string uid);
	~ChatBot();
private:
	std::string						m_channel;
	std::string						m_name;
	std::string						m_uid;

	std::map<std::string, std::function<void(std::string)>>	m_localCommands;
	Client& m_client;
};


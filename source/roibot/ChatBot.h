#pragma once

#include <map>
#include <string>
#include <functional>

class Client;
class ChatBot
{
public:
	ChatBot(Client& client, std::string name, std::string uid, std::string channel);
	~ChatBot();

	void command(std::string command);
	void send(std::string message);
	
	void addLocalCommand(std::string name, std::function<void(std::string)> command);
	void addChatCommand(std::string name, std::function<void(std::string)> command);

	void setChannel(std::string channel){ m_channel = channel; };
	std::string getName(){ return m_name; };
	std::string getUID(){ return m_uid; };
	std::string getChannel(){ return m_channel; };
	void setName(std::string n){ m_name = n; };
private:
	void onChat(std::string chat);
	std::string						m_channel;
	std::string						m_name;
	std::string						m_uid;

	std::map<std::string, std::function<void(std::string)>>	m_localCommands;
	std::map<std::string, std::function<void(std::string)>>	m_chatCommands;

	Client& m_client;
};


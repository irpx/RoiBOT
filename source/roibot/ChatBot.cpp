#include "ChatBot.h"


ChatBot::ChatBot(Client& client, std::string name, std::string uid)
	: m_client(client)
	, m_name(name)
	, m_uid(uid)
{
}


ChatBot::~ChatBot()
{
}

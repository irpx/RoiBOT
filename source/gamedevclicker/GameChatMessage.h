#pragma once
#include <string>
#include "GameMessage.h"
class GameChatMessage :
	public GameMessage
{
public:
	GameChatMessage(std::string msg, std::string sender);
	virtual ~GameChatMessage();
	virtual void execute(GameDevClickerMMO* game) override;
	std::string m_chatMessage;
	std::string m_senderName;
};


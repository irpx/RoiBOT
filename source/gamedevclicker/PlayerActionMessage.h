#pragma once
#include "GameMessage.h"
#include <string>
class PlayerActionMessage :
	public GameMessage
{
public:
	PlayerActionMessage(std::string action, std::string player, std::string target);
	~PlayerActionMessage();
	std::string actionName;
	std::string playerName;
	std::string targetName;
};


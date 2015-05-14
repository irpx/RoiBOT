#include "PlayerActionMessage.h"

PlayerActionMessage::PlayerActionMessage(std::string action, std::string player, std::string target)
	: GameMessage(gameMessageType::PLAYER_ACTION)
	, playerName(player)
	, actionName(action)
	, targetName(target)
{
}

PlayerActionMessage::~PlayerActionMessage()
{
}

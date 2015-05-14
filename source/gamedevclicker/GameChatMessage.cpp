#include "GameChatMessage.h"
#include "GameDevClickerMMO.h"

GameChatMessage::GameChatMessage(std::string msg, std::string sender)
    : GameMessage(gameMessageType::GAME_CHAT)
    , m_chatMessage(msg)
    , m_senderName(sender)
{
}

GameChatMessage::~GameChatMessage()
{
}

void GameChatMessage::execute(GameDevClickerMMO* game)
{
    m_state = GameMessage::messageState::CLIENT_INPUT_PROCESSED;
    game->gameLog(m_chatMessage, m_senderName);
    game->addClientOutput(std::make_shared<GameChatMessage>(m_chatMessage, m_senderName));
}
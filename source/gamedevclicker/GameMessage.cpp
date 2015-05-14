#include "GameMessage.h"

GameMessage::GameMessage(gameMessageType type)
    : m_type(type)
    , m_state(messageState::CLIENT_INPUT_SENT)
{
}

GameMessage::~GameMessage()
{
}
void GameMessage::execute(GameDevClickerMMO* game)
{
    m_state = messageState::CLIENT_INPUT_PROCESSED;
}
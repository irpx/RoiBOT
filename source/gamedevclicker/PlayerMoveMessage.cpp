#include "PlayerMoveMessage.h"
#include "PlayerActionMessage.h"
#include "GameDevClickerMMO.h"
#include "Player.h"

PlayerMoveMessage::PlayerMoveMessage(std::string name, int playerID, int x, int y)
    : GameMessage(gameMessageType::PLAYER_MOVE)
    , m_name(name)
	, m_playerID(playerID)
    , m_x(x)
    , m_y(y)
{
}

PlayerMoveMessage::~PlayerMoveMessage()
{
}

void PlayerMoveMessage::execute(GameDevClickerMMO* game)
{
    auto players = game->getPlayers();
    for (auto player : players)
    {
        if (player->getID() != m_playerID) continue;

		m_name = player->getName().c_str();

        int x = m_x;
        int y = m_y;
        
		int xlimit = 0;
        int ylimit = 0;
        std::shared_ptr<Entity> targetEntity = nullptr;
        if (player->getParent()->getType() == Entity::entityType::ROOM)
        {
            auto container = dynamic_cast<Room*>(player->getParent());
            targetEntity = container->getEntityAtBlock(x, y);
            auto size = container->getSize();
            xlimit = std::get<0>(size);
            ylimit = std::get<0>(size);
            if (targetEntity == player)
            {
                targetEntity == nullptr;
            }
        }

        if (targetEntity == nullptr)
        {
            if (x < 0) x = 0;
            if (y < 0) y = 0;

            player->x = x < xlimit ? x : xlimit - 1;
            player->y = y < ylimit ? y : ylimit - 1;
        }
        else
        {
            game->addClientOutput(std::make_shared<PlayerActionMessage>("Attacks", player->getName().c_str(), targetEntity->getName().c_str()));
        }

        //Need to reset movement on client side?
        game->addClientOutput(std::make_shared<PlayerMoveMessage>(m_name, m_playerID, player->x, player->y));

        return;
    }
}
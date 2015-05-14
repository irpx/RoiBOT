#pragma once
#include <string>
#include "GameMessage.h"
class PlayerMoveMessage :
	public GameMessage
{
public:
	PlayerMoveMessage(std::string name,int playerID,int x=0,int y=0);
	~PlayerMoveMessage();
	virtual void execute(GameDevClickerMMO* game) override;
	std::string m_name;
	int m_playerID;
	int m_x;
	int m_y;
};


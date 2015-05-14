#pragma once
class GameDevClickerMMO;
class GameMessage
{
public:
	enum class gameMessageType
	{
		GAME_CHAT,
		PLAYER_MOVE,
		PLAYER_ACTION,
		ENEMY_ADD,
		ENEMY_ACTION,
		STATISTICS_UPDATE,
		GAME_EVENT
	};
	enum class messageState
	{
		CLIENT_INPUT_SENT,
		CLIENT_INPUT_PROCESSED,
		CLIENT_OUTPUT_SENT,
		CLIENT_OUTPUT_PROCESSED
	};
	GameMessage(gameMessageType type);
	virtual ~GameMessage();
	virtual void execute(GameDevClickerMMO* game);
	gameMessageType	m_type;
	messageState m_state;
};


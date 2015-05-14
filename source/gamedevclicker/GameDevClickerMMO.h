#pragma once
#include <atomic>
#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <sstream>
#include "StudioDungeon.h"
#include "GameStats.h"
#include "GameMessage.h"

class Player;
class GameDevClickerMMO
{
public:
	GameDevClickerMMO();
	~GameDevClickerMMO();
	void init();
	void addPlayer(std::string name,int id);
	void removePlayer(std::string name,int id);
	std::vector<std::shared_ptr<Player>> getPlayers();
	std::shared_ptr<Player> getPlayer(std::string name);
	std::shared_ptr<Player> getPlayer(int id);
	void update();
	bool isRunning() { return m_running; };
	void log(std::string msg);
	void gameLog(std::string msg,std::string sender="SYSTEM");

	std::vector<std::shared_ptr<GameMessage>>& getClientOutputQueue() { return m_clientOutputQueue; };
	std::vector<std::shared_ptr<GameMessage>>& getClientInputQueue() { return m_clientInputQueue; };
	void addClientOutput(std::shared_ptr<GameMessage> output);
	void addClientInput(std::shared_ptr<GameMessage> input);

	StudioDungeon* getDungeon() { return &m_dungeon; };
	std::stringstream&	getGameLog() { return m_gameLog; };
private:
	std::atomic<bool>						m_running;
	GameStats					m_stats;
	StudioDungeon				m_dungeon;
	std::vector<std::shared_ptr<Player>>		m_players;
	std::vector<std::shared_ptr<Entity>>		m_entities;
	std::chrono::system_clock::time_point		m_updateClock;

	std::vector<std::shared_ptr<GameMessage>>	m_clientOutputQueue;
	std::vector<std::shared_ptr<GameMessage>>	m_clientInputQueue;

	std::stringstream			m_gameLog;
};


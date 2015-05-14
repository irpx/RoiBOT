#include "Player.h"
#include <algorithm>

Player::Player(std::string name)
    : Entity()
    , ENTITY_PROPERTY_DECL(health, 100)
    , ENTITY_PROPERTY_DECL(sanity, 100)
    , ENTITY_PROPERTY_DECL(focus, 1.f)
    , ENTITY_PROPERTY_DECL(energy, 100)
    , ENTITY_PROPERTY_DECL(depression, 0)
{
    m_name = name;
    m_type = entityType::PLAYER;

	auto task = new PlayerTask(this, "Get a Task", "Find something to do.");
	addTask(task);
}

Player::~Player()
{
	tasks.clear();
}

void Player::addTask(PlayerTask* task)
{
	tasks.push_back(task);
}

void Player::removeTask(PlayerTask* task)
{
	tasks.erase(
		std::remove_if(tasks.begin(), tasks.end(),[&](PlayerTask* t) { return t->name == task->name; })
	);
}
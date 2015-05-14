#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Entity.h"
#include "PlayerTask.h"

class Player : public Entity
{
public:
    Player(std::string name);
    ~Player();

    EntityProperty<int> health;
    EntityProperty<int> sanity;
    EntityProperty<float> focus;
    EntityProperty<int> energy;
    EntityProperty<int> depression;

	void addTask(PlayerTask* task);
	void removeTask(PlayerTask* task);

	std::vector<PlayerTask*>	tasks;
};

#include "PlayerTask.h"

int PlayerTask::nextID = 0;

PlayerTask::PlayerTask(Entity* o, std::string n, std::string d)
	: owner(o)
	, name(n)
	, id(nextID++)
	, about(d)
{
}

PlayerTask::~PlayerTask()
{
}

bool PlayerTask::isCompleted()
{
	bool success = true;
	for (auto role : roles)
	{
		if (!role->isSatisfied())
		{
			success = false;
			break;
		}
	}
	return success;
}
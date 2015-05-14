#pragma once
#include <string>
#include <sstream>
#include <vector>

#include "Entity.h"

enum class taskAction
{
	GENERIC_TARGET,
	GIVE_ITEM,
	GET_ITEM,
	RECEIVE_ACTION_TO,
	GOTO_TO,
	TALK_TO,
	ABOUT_ACTION,
	FIX_THIS,
	GET_ITEM_FROM,
	RECEIVE_ACTION_FROM
};

class TaskRole
{
public:
	TaskRole(Entity* e) :entity(e){};
	virtual ~TaskRole(){};
	bool isSatisfied() { return false; };

	taskAction	action;

	Entity* entity;

	static std::string getActionName(taskAction& act)
	{
		switch (act)
		{
		case taskAction::GENERIC_TARGET:
			return " ";
			break;
		case taskAction::GIVE_ITEM:
			return "Give ";
			break;
		case taskAction::RECEIVE_ACTION_TO:
			return " to ";
			break;
		case taskAction::GOTO_TO:
			return "Go to ";
			break;
		case taskAction::TALK_TO:
			return "Talk to ";
			break;
		case taskAction::ABOUT_ACTION:
			return " about ";
			break;
		case taskAction::FIX_THIS:
			return "Fix ";
			break;
		case taskAction::GET_ITEM:
			return "Get ";
			break;
		case taskAction::RECEIVE_ACTION_FROM:
			return " from ";
			break;
		default:
			return "Snap ";
			break;
		}
	}
};

class PlayerTask
{
public:
	PlayerTask(Entity* o, std::string n, std::string d);
	~PlayerTask();

	std::string getDescription()
	{
		std::stringstream output;

		output << owner->getName().c_str() << ", ";

		for (auto role : roles)
		{
			output << TaskRole::getActionName(role->action);
			if (role->entity != nullptr)
			{
				output << role->entity->getName().c_str();
			}
		}
		return output.str();
	};

	Entity*	owner;
	Entity*	givedBy;//TODO refactor to shared_ptrs
	std::string name;
	std::string about;

	bool isCompleted();

	std::vector<std::shared_ptr<TaskRole>> roles;

	int id;

	static int nextID;
};
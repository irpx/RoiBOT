#pragma once
#include "EntityContainer.h"
#include "Room.h"

class StudioDungeon : public EntityContainer
{
public:
	StudioDungeon();
	~StudioDungeon();
	void addRoom(std::shared_ptr<Room> room);
	std::vector<std::shared_ptr<Room>>& getRooms() { return m_rooms; };
private:
	std::vector<std::shared_ptr<Room>> m_rooms;
};


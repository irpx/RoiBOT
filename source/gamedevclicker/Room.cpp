#include "Room.h"
#include "Entity.h"

Room::Room(int w, int h)
    : EntityContainer()
    , width(w)
    , height(h)
{
    m_type = entityType::ROOM;
}

Room::~Room()
{
}
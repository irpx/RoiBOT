#include "StudioDungeon.h"
#include <cstdlib>
#include <ctime>

StudioDungeon::StudioDungeon()
    : EntityContainer()
{
    std::srand((unsigned)std::time(0));
    unsigned w = 3 + (std::rand() % 15);
    std::srand((unsigned)std::time(0) + w);
    int h = 3 + (std::rand() % 15);
    auto room = std::make_shared<Room>(12, 12);
    addEntity(room);
    room->getName() = "Programmer's cave";
    addRoom(room);

    std::shared_ptr<Entity> e = std::make_shared<Entity>();
    e->getType() = Entity::entityType::WALL;
    e->x = 3;
    e->y = 3;
    e->getName() = "wall";
    room->addEntity(e);

    e = std::make_shared<Entity>();
    e->getType() = Entity::entityType::WALL;
    e->x = 8;
    e->y = 3;
    e->getName() = "wall";
    room->addEntity(e);

    e = std::make_shared<Entity>();
    e->getType() = Entity::entityType::WALL;
    e->x = 3;
    e->y = 8;
    e->getName() = "wall";
    room->addEntity(e);

    e = std::make_shared<Entity>();
    e->getType() = Entity::entityType::WALL;
    e->x = 8;
    e->y = 8;
    e->getName() = "wall";
    room->addEntity(e);

    e = std::make_shared<Entity>();
    e->getType() = Entity::entityType::WORKSTATION;
    e->x = 2;
    e->y = 2;
    e->getName() = "workstation";
    room->addEntity(e);

    e = std::make_shared<Entity>();
    e->getType() = Entity::entityType::WORKSTATION;
    e->x = 4;
    e->y = 2;
    e->getName() = "workstation";
    room->addEntity(e);

    e = std::make_shared<Entity>();
    e->getType() = Entity::entityType::WORKSTATION;
    e->x = 6;
    e->y = 2;
    e->getName() = "workstation";
    room->addEntity(e);

    e = std::make_shared<Entity>();
    e->getType() = Entity::entityType::WORKSTATION;
    e->x = 8;
    e->y = 2;
    e->getName() = "workstation";
    room->addEntity(e);
}

StudioDungeon::~StudioDungeon()
{
}

void StudioDungeon::addRoom(std::shared_ptr<Room> room)
{
    m_rooms.push_back(room);
    addEntity(room);
}
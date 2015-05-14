#include "EntityContainer.h"
#include <algorithm>

EntityContainer::EntityContainer()
    : Entity()
{
    m_type = entityType::CONTAINER;
}

EntityContainer::~EntityContainer()
{
}

void EntityContainer::removeEntity(std::shared_ptr<Entity> e)
{
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(), [&](std::shared_ptr<Entity> entity)
    {
        bool match = entity == e;
        if (match)
        {
            e->setParent(nullptr);
        }
        return match;
    }),
        m_entities.end()
        );
}

void EntityContainer::addEntity(std::shared_ptr<Entity> e)
{
    e->setParent(this);
    m_entities.push_back(e);
};

std::shared_ptr<Entity> EntityContainer::getEntityAtBlock(int x, int y)
{
    for (auto entity : m_entities)
    {
        if (entity->x == x && entity->y == y)
        {
            return entity;
        }
    }
    return nullptr;
}
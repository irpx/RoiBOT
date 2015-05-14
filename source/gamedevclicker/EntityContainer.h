#pragma once
#include <vector>
#include <memory>
#include "Entity.h"
class EntityContainer : public Entity
{
public:
    EntityContainer();
    virtual ~EntityContainer();
    virtual void addEntity(std::shared_ptr<Entity> e);
    virtual void removeEntity(std::shared_ptr<Entity> e);
    std::vector<std::shared_ptr<Entity>>& getEntities() { return m_entities; };
    std::shared_ptr<Entity> getEntityAtBlock(int x, int y);
private:
    std::vector<std::shared_ptr<Entity>>	m_entities;
};

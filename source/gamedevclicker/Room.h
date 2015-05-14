#pragma once
#include <map>
#include "EntityContainer.h"

class Room : public EntityContainer
{
public:
    Room(int w, int h);
    virtual ~Room();
    std::tuple<int, int> getSize()
    {
        return std::make_tuple(width, height);
    };
    //virtual void addEntity(std::shared_ptr<Entity> e);
private:
    int	width;
    int	height;
    std::map<int, std::map<int, std::shared_ptr<Entity>>> m_entityMap;
};
#include "EntityProperties.h"
#include "Entity.h"

EntityPropertyBase::~EntityPropertyBase()
{
}

EntityPropertyBase::EntityPropertyBase(Entity* owner, std::string n)
    : name(n)
{
    owner->setProperty(this);
}
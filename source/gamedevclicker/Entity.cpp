#include "Entity.h"

Entity::Entity()
	:	m_parent(nullptr)
	,	m_name("nullEntity")
	,	m_type(entityType::NORMAL)
{
	x = 0;
	y = 0;
}

Entity::~Entity()
{
}

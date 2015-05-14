#pragma once
#include <string>
#include <memory>

#include "EntityProperties.h"
class Entity
{
public:
    enum class entityType
    {
        NORMAL,
        CONTAINER,
        ROOM,
        NPC,
        PLAYER,
        STATIC,
        WALL,
        DYNAMIC,
        WORKSTATION
    };
    Entity();
    virtual ~Entity();
    int x;
    int y;
    void setParent(Entity* e) { m_parent = e; };
    Entity* getParent() { return m_parent; };
    std::string& getName() { return m_name; };
    entityType& getType() { return m_type; };
	void setProperty(EntityPropertyBase* property)
    {
        m_properties.insert(std::make_pair(property->name, property));
    };
	EntityPropertyBase* getProperty(std::string name)
    {
        auto it = m_properties.find(name);
        if (it == m_properties.end())
        {
            return nullptr;
        }
        return it->second;
    };
	void removeProperty(std::string name)
	{
		auto it = m_properties.find(name);
		if (it == m_properties.end())
		{
			m_properties.erase(it);
		}
	};
	std::map<std::string, EntityPropertyBase*>& getProperties() { return m_properties; };
	int getID() { return m_id; };
	void setID(int id) { m_id = id; };
protected:
	std::map<std::string, EntityPropertyBase*> m_properties;
    entityType			m_type;
    Entity*				m_parent;
    std::string			m_name;
	int					m_id;
};

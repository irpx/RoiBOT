#pragma once

#include <string>
#include <map>

#define ENTITY_PROPERTY_DECL(name,defaultValue) name(this,#name,defaultValue)

class Entity;
class EntityPropertyBase
{
public:
    EntityPropertyBase(Entity* owner, std::string name);
    ~EntityPropertyBase();
    virtual std::string getValue() = 0;// { return ""; };
    virtual bool isNumber() = 0;
    std::string name;
protected:
    bool _isNumber(std::string i) { return false; };
    bool _isNumber(int i) { return true; };
    bool _isNumber(float i) { return true; };
    bool _isNumber(unsigned i) { return true; };
};

template<typename T>
class EntityProperty : public EntityPropertyBase
{
public:
    EntityProperty(Entity* owner, std::string name, T value)
        : EntityPropertyBase(owner, name)
        , m_value(value)
    {
    };
    virtual bool isNumber() { return _isNumber(m_value); };

    T& value() { return m_value; };
    virtual std::string getValue() { return std::to_string(m_value); };
private:
    T m_value;
};

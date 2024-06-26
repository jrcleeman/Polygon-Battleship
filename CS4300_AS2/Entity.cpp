#include "Entity.h"

// Entity constructor
Entity::Entity(const size_t i, const std::string& t)
	:m_id(i), m_tag(t) 
{
}

// Get entity active flag
bool Entity::isActive() const
{
	return m_active;
}

// Get entity tag
const std::string& Entity::tag() const
{
	return m_tag;
}

// Get entity ID
const size_t Entity::id() const
{
	return m_id;
}

// Destroy entity
void Entity::destroy()
{
	m_active = false;
}
#include "EntityManager.h"

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
	//Add entities from m_entitiesToAdd at the proper locations
	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e); // Add entity to the vector of all entities
		m_entityMap[e->tag()].push_back(e); // Add entity to the vector map with the tag as a key
	}
	m_entitiesToAdd.clear();// Clear entities to add

	//remove dead entities from each vector in the entity map
	//c++ way of iterating through [key, value] pairs in a map
	for (auto& x : m_entityMap)
	{
		removeDeadEntities(x.second);
	}
	
	removeDeadEntities(m_entities);
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	//TODO remove all dead entities from the input vector 
	//this is called by the update function
	for (int i = vec.size() - 1; i >= 0; i--) {
		if (vec.at(i)->isActive() == false)
		{
			vec.erase(vec.begin() + i);
		}
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag)); // Create the shared entity ptr
	m_entitiesToAdd.push_back(entity); //Push onto entities to add vector
	return entity;
}

// Return all entities
const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

// Get entities of specifc type
const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}
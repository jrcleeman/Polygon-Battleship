#pragma once
#include "Common.h"

#include "Components.h"
#include <memory>


class Entity
{
	friend class EntityManager;

	bool        m_active = true;
	size_t      m_id     = 0;
	std::string m_tag    = "default"; // Player, Enemy, Bullet

	// Constructor (private so entities cannot be accidentally created)
	Entity(const size_t id, const std::string& tag);

public:
	//Component pointers
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifespan> cLifespan;

	//Private member access functions
	bool isActive() const; // Get active bool
	const std::string& tag() const; // Get tag
	const size_t id() const; // Get ID
	void destroy(); // Destroy object
};
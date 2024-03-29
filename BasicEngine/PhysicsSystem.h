#pragma once
//==========================================================================================
// File Name: PhysicsSystem.h
// Author: Brian Blackmon
// Date Created: 10/30/2019
// Purpose: 
// Handles all of the physics updates for the system.
//==========================================================================================
#include "Quadtree.h"
#include "Velocity.h"
#include "CollisionBox.h"
#include <map>

class PhysicsSystem
{
public:
	PhysicsSystem(const int maxLevels,
		const int maxObjects,
		const Rectangle& bounds);
	~PhysicsSystem();

	CollisionBox* getCollisionBox(const int collisionBoxID) const;
	Velocity* getVelocity(const int velocityID);

	CollisionBox* createCollisionBox(const int collisionBoxID,
		const Rectangle& box,
		const bool solid);

	void update(const float delta);

private:
	std::map<int, CollisionBox*> m_collisionBoxes;
	std::map<int, Velocity*> m_velocity;

	Quadtree *m_collisionGrid;

	const float m_FRICTION = 1.0f;

	void handleMovement(const int boxID,
		CollisionBox* box,
		const Vector2D& movement);

	void moveOutside(CollisionBox* box,
		CollisionBox* collision);

	Vector2D applyFriction(const Vector2D& velocity,
		const float delta) const;

	void cleanUp();
};


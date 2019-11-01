#include "PhysicsSystem.h"
#include "BBMath.h"
#include "Collision.h"

PhysicsSystem::PhysicsSystem(const int maxLevels,
	const int maxObjects,
	const Rectangle& bounds)
	:m_collisionGrid(NULL)
{
	int workingLevels = maxLevels;

	if (workingLevels <= 0)
	{
		workingLevels = 1;
	}

	int workingObjects = maxObjects;

	if (workingObjects <= 0)
	{
		workingObjects = 1;
	}

	m_collisionGrid =
		new Quadtree(maxObjects, maxLevels, 0, bounds, NULL);
}

PhysicsSystem::~PhysicsSystem()
{
	cleanUp();
}

//=============================================================================
// Function: CollisionBox* getCollisionBox(const int) const
// Description:
// Gets the collision box for the provided ID.
// Parameters:
// const int collisionBoxID - The ID of the collision box to get.
// Output:
// CollisionBox*
// On success - Returns a collision box.
// On failure - Returns NULL.
//=============================================================================
CollisionBox* PhysicsSystem::getCollisionBox(const int collisionBoxID) const
{
	CollisionBox *collision = NULL;

	auto mit = m_collisionBoxes.find(collisionBoxID);

	if (mit != m_collisionBoxes.end())
	{
		collision = mit->second;
	}

	return collision;
}

//=============================================================================
// Function: Velocity* getVelocity(const int)
// Description:
// Finds the velocity component with the specified ID.
// If it doesn't exist, it creates it.
// Parameters:
// const int velocityID - The id of the velocity to find.
// Output:
// Velocity*
// On success - Returns a pointer to the found velocity.
// On failure - Returns NULL.
//=============================================================================
Velocity* PhysicsSystem::getVelocity(const int velocityID)
{
	Velocity *vel = NULL;

	auto mit = m_velocity.find(velocityID);

	if (mit != m_velocity.end())
	{
		vel = mit->second;
	}
	else
	{
		vel = new Velocity();
	}

	return vel;
}

//=============================================================================
// Function: CollisionBox* createCollisionBox(
// const int collisionBoxID,
// const Rectangle& box,
// const bool solid)
// Description:
// Creates a collision box for the ID if one doesn't already
// exist.
// Parameters:
// const int collisionBoxID - The id to create the box under.
// const Rectangle& box - The size of the collision box.
// const bool solid - The solid state of the collision box.
// Output:
// CollisionBox*
// On success - Returns the created/found collision box.
// On failure - Returns NULL.
//=============================================================================
CollisionBox* PhysicsSystem::createCollisionBox(const int collisionBoxID,
	const Rectangle& box,
	const bool solid)
{
	CollisionBox *collision = NULL;

	auto mit = m_collisionBoxes.find(collisionBoxID);

	if (mit != m_collisionBoxes.end())
	{
		collision = mit->second;
	}
	else
	{
		collision = new CollisionBox(box, solid);

		m_collisionBoxes.insert(std::make_pair(collisionBoxID, collision));

		m_collisionGrid->addEntity(EntityData(collisionBoxID, box));
	}

	return collision;
}

//=============================================================================
// Function: void update(const float)
// Description:
// Updates all of the physics objects.
// Parameters:
// const float delta - The time since last update.
//=============================================================================
void PhysicsSystem::update(const float delta)
{
	if (m_collisionGrid)
	{
		auto mit = m_velocity.begin();

		while (mit != m_velocity.end())
		{
			Velocity *velocity = mit->second;
			CollisionBox *box = getCollisionBox(mit->first);

			if (velocity->getMagnitude() != 0.0f && box)
			{
				Vector2D movement(velocity->getDirection().m_x, 
					velocity->getDirection().m_y);

				movement *= Vector2D(delta, delta);

				handleMovement(mit->first, box, movement);

				velocity->addVelocity(applyFriction(velocity->getDirection(), delta));
			}

			mit++;
		}
	}
}

//=============================================================================
// Function: void handleMovement(const int,
// CollisionBox*,
// const Vector2D&)
// Description:
// Handles the movement for the collision box.
// Parameters:
// const int boxID - The id of the box to move.
// CollisionBox* box - The box to move.
// const Vector2D& movement - The movement to try to make.
//=============================================================================
void PhysicsSystem::handleMovement(const int boxID,
	CollisionBox* box,
	const Vector2D& movement)
{
	if (box)
	{
		Vector2D startPos = box->getPosition();
		Vector2D endPos = startPos + movement;
		Line distance(startPos, endPos);

		// Checks to see if there are any collisions on the move path.
		std::vector<EntityData> lineSearch
			= m_collisionGrid->search(distance);

		CollisionBox *collision = NULL;
		CollisionBox *temp = NULL;

		Vector2D collisionPoint(-1.0f, -1.0f);

		for (unsigned int i = 0; i < lineSearch.size(); i++)
		{
			if (lineSearch[i].m_id != boxID)
			{
				temp = getCollisionBox(lineSearch[i].m_id);

				if (temp->getSolid())
				{
					if (collision)
					{
						Vector2D tempIntersect = intersectPoint(temp->getBox(), distance);

						if (totalDistance(startPos, tempIntersect) < 
							totalDistance(startPos, collisionPoint))
						{
							collisionPoint = tempIntersect;
							collision = temp;
						}
					}
					else
					{
						collision = temp;
						
						collisionPoint = intersectPoint(collision->getBox(), distance);
					}
				}
			}
		}

		// If a collision was made, move up to the collision point.
		if (collision &&
			0.0f <= collisionPoint.m_x &&
			0.0f <= collisionPoint.m_y)
		{
			endPos = collisionPoint;
		}

		box->setPosition(endPos);

		std::vector<EntityData> movedCollisions =
			m_collisionGrid->search(box->getBox());

		for (unsigned int i = 0; i < movedCollisions.size(); i++)
		{
			if (movedCollisions[i].m_id != boxID)
			{
				temp = getCollisionBox(movedCollisions[i].m_id);

				if (temp && temp->getSolid())
				{
					moveOutside(box, temp);
				}
			}
		}

		endPos = box->getPosition();

		box->setPosition(startPos);

		m_collisionGrid->removeEntity(EntityData(boxID, box->getBox()));
		
		box->setPosition(endPos);

		m_collisionGrid->addEntity(EntityData(boxID, box->getBox()));
	}
}

//=============================================================================
// Function: void moveOutside(CollisionBox*,
// CollisionBox*,
// const Vector2D&)
// Description:
// Moves the box outside of the box it's colliding with.
// Parameters:
// CollisionBox* box - The box to move.
// CollisionBox* collision - The box it's inside.
//=============================================================================
void PhysicsSystem::moveOutside(CollisionBox* box,
	CollisionBox* collision)
{
	if (box && collision)
	{
		Vector2D updatedPosition = box->getPosition();
		Vector2D overlap = overlapAmount(box->getBox(), collision->getBox());
		// find the smallest move
		// If you can just move the x, move the x
		// If you can just move the y, move the y
		// Otherwise, move both.
		if (absoluteValue(overlap.m_x) <= 
			absoluteValue(overlap.m_y))
		{
			updatedPosition.m_x += overlap.m_x;
		}
		else
		{
			updatedPosition.m_y += overlap.m_y;
		}

		box->setPosition(updatedPosition);
	}
}

//=============================================================================
// Function: Vector2D applyFriction(const Vector2D&,
// const float) const
// Description:
// Applies friction to a velocity, moving it closer to 0.
// Parameters:
// const Vector2D& velocity - The velocity to apply friction to.
// const float delta - The time passed since last update.
// Output:
// Vector2D
// Returns the resulting vector from applying friction.
//=============================================================================
Vector2D PhysicsSystem::applyFriction(const Vector2D& velocity,
	const float delta) const
{
	float xChange = m_FRICTION * delta;
	float yChange = m_FRICTION * delta;

	if (absoluteValue(velocity.m_x) < xChange)
	{
		xChange = absoluteValue(velocity.m_x);
	}

	if (absoluteValue(velocity.m_y) < yChange)
	{
		yChange = absoluteValue(velocity.m_y);
	}

	if (0.0f < velocity.m_x)
	{
		xChange *= -1.0f;
	}

	if (0.0f < velocity.m_y)
	{
		yChange *= -1.0f;
	}

	return Vector2D(xChange, yChange);
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Deletes all of the components, collision grid, and frees memory.
//=============================================================================
void PhysicsSystem::cleanUp()
{
	if (m_collisionGrid)
	{
		delete m_collisionGrid;
	}

	auto collisionMit = m_collisionBoxes.begin();

	while (collisionMit != m_collisionBoxes.end())
	{
		delete collisionMit->second;
		collisionMit = m_collisionBoxes.erase(collisionMit);

		if (collisionMit != m_collisionBoxes.end())
		{
			collisionMit++;
		}
	}

	auto velMit = m_velocity.begin();

	while (velMit != m_velocity.end())
	{
		delete velMit->second;
		velMit = m_velocity.erase(velMit);

		if (velMit != m_velocity.end())
		{
			velMit++;
		}
	}
}
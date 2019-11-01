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

				handleMovement(box, movement);

				velocity->addVelocity(applyFriction(velocity->getDirection(), delta));
			}

			mit++;
		}
	}
}

//=============================================================================
// Function: void handleMovement(CollisionBox*,
// const Vector2D&)
// Description:
// Handles the movement for the collision box.
// Parameters:
// CollisionBox* box - The box to move.
// const Vector2D& movement - The movement to try to make.
//=============================================================================
void PhysicsSystem::handleMovement(CollisionBox* box,
	const Vector2D& movement)
{
	Vector2D startPos = box->getPosition();
	Vector2D endPos = startPos + movement;

	std::vector<EntityData> collisionData =
		m_collisionGrid->search(Line(startPos, endPos));

	CollisionBox *collision = NULL;
	CollisionBox *temp = NULL;

	for (unsigned int i = 0; i < collisionData.size(); i++)
	{
		temp = getCollisionBox(collisionData[i].m_id);

		if (collision != NULL)
		{
			if (temp->getSolid())
			{
				if (totalDistance(temp->getPosition(), startPos) <
					totalDistance(collision->getPosition(), startPos))
				{
					collision = temp;
				}
			}
		}
		else
		{
			if (temp->getSolid())
			{
				collision = temp;
			}
		}
	}

	if (collision == NULL)
	{
		box->setPosition(endPos);

		std::vector<EntityData> confirmData =
			m_collisionGrid->search(box->getBox());

		if (0 < confirmData.size())
		{
			box->setPosition(startPos);

			for (unsigned int i = 0; i < confirmData.size(); i++)
			{
				if (collision != NULL)
				{
					if (temp->getSolid())
					{
						if (totalDistance(temp->getPosition(), endPos) <
							totalDistance(collision->getPosition(), endPos))
						{
							collision = temp;
						}
					}
				}
				else
				{
					if (temp->getSolid())
					{
						collision = temp;
					}
				}
			}

			if (collision)
			{

			}
		}
	}
	else
	{
		Vector2D collisionPoint = intersectPoint(collision->getBox(), Line(startPos, endPos));
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
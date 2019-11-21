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
		m_velocity.insert(std::make_pair(velocityID, vel));
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

				handleMovement(mit->first, box, movement * delta);

				//velocity->addVelocity(applyFriction(velocity->getDirection(), delta));
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

		if (startPos.m_x == 0.0f && startPos.m_y == 0.0f)
		{
			int i = 0;
		}

		Vector2D startTopPos = box->getPosition();
		startTopPos.m_y = box->getBox().getMinY();

		Vector2D startBottomPos = box->getPosition();
		startBottomPos.m_y = box->getBox().getMaxY();

		Vector2D endPos = startPos + movement;
		Vector2D endTopPos = startTopPos + movement;
		Vector2D endBottomPos = startBottomPos + movement;

		Line distance(startPos, endPos);
		Line topDistance(startTopPos, endTopPos);
		Line bottomDistance(startBottomPos, endBottomPos);

		roundLine(distance);

		// Checks to see if there are any collisions on the move path.
		std::unordered_map<int, EntityData> lineSearch;

		m_collisionGrid->search(distance, lineSearch);

		CollisionBox *collision = NULL;
		CollisionBox *temp = NULL;

		Vector2D collisionPoint(-1.0f, -1.0f);

		auto mit = lineSearch.begin();

		while (mit != lineSearch.end())
		{
			if (mit->second.m_id != boxID)
			{
				temp = getCollisionBox(mit->second.m_id);

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

			mit++;
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
					if (movement.m_x < -100.0f)
					{
						int i = 0;
					}
					moveOutside(box, temp, movement);
				}
			}
		}

		endPos = box->getPosition();

		box->setPosition(startPos);

		m_collisionGrid->removeEntity(EntityData(boxID, box->getBox()));
		
		if (endPos.m_x < 0.0f)
		{
			int i = 0;
		}

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
	CollisionBox* collision,
	const Vector2D& movement)
{
	if (box && collision)
	{
		Vector2D updatedPosition = box->getPosition();

		Rectangle boxRect = box->getBox();
		Rectangle collideRect = collision->getBox();

		// Get the corners of rect a that are
		// in the movement direction.

		std::vector<Vector2D> aClosestPoints;

		aClosestPoints = getClosestPoints(boxRect, box->getPosition() + (movement));

		// A REALLY big mod, so the lines are long.
		Vector2D moveMod = movement * 10000.0f;
		Vector2D moveNormal = movement / sqrt(movement.m_x * movement.m_x + movement.m_y * movement.m_y);

		if (aClosestPoints[0].m_x == 0.0f &&
			aClosestPoints[0].m_y == 0.0f)
		{
			int i = 0;
		}

		if (dotProduct(collideRect.getCenter() - boxRect.getCenter(), movement) < 0.0f)
		{
			moveMod = moveMod * -1.0f;
		}

		// Draw a line and find the farthest collision point on the line.
		Line firstPointRay(aClosestPoints[0], aClosestPoints[0] - moveMod);
		Line secondPointRay(aClosestPoints[1], aClosestPoints[1] - moveMod);

		std::vector<Vector2D> firstCollisions = collisionPoints(collideRect, firstPointRay);
		std::vector<Vector2D> secondCollisions = collisionPoints(collideRect, secondPointRay);

		Vector2D wallNormal;
		
		// Handle the first set of collisions.
		if (firstCollisions.size() != 0 ||
			secondCollisions.size() != 0)
		{
			Vector2D startingPoint;
			Vector2D farthestPoint(-1.0f, -1.0f);
			float farthestDist = -1.0f;

			if (firstCollisions.size() != 0)
			{
				startingPoint = firstPointRay.m_start;
				farthestPoint = firstCollisions[0];
				farthestDist = totalDistance(firstPointRay.m_start, farthestPoint);

				for (unsigned int i = 0; i < firstCollisions.size(); i++)
				{
					if (farthestDist <
						totalDistance(firstPointRay.m_start, firstCollisions[i]))
					{
						farthestPoint = firstCollisions[i];
						farthestDist = totalDistance(firstPointRay.m_start, farthestPoint);
					}
				}
			}

			if (secondCollisions.size() != 0)
			{
				if (farthestPoint.m_x == -1.0f &&
					farthestPoint.m_y == -1.0f)
				{
					farthestPoint = secondCollisions[0];
					farthestDist = totalDistance(secondPointRay.m_start, farthestPoint);
					startingPoint = secondPointRay.m_start;
				}

				for (unsigned int i = 0; i < secondCollisions.size(); i++)
				{
					if (farthestDist <
						totalDistance(secondPointRay.m_start, secondCollisions[i]))
					{
						startingPoint = secondPointRay.m_start;
						farthestPoint = secondCollisions[i];
						farthestDist = totalDistance(secondPointRay.m_start, farthestPoint);
					}
				}
			}

//			farthestPoint.m_x = roundf(farthestPoint.m_x);
//			farthestPoint.m_y = roundf(farthestPoint.m_y);

			// Now that we have the furthest point, we need to move back that amount
			// Add in the one step further back, so no collisions are happening.
			Vector2D moveAmount = farthestPoint - startingPoint - (movement * -0.001f);

			Vector2D rounded = box->getPosition() + moveAmount;

			rounded.m_x = roundf(rounded.m_x);
			rounded.m_y = roundf(rounded.m_y);

			box->setPosition(rounded);

			if (isnan(box->getPosition().m_x))
			{
				int i = 0;
			}

			std::vector<Line> topLines = collisionLines(collideRect, firstPointRay);
			std::vector<Line> bottomLines = collisionLines(collideRect, secondPointRay);

			Vector2D wallNormal;

			if (topLines.size() != 0 && bottomLines.size() != 0)
			{
				for (unsigned int i = 0; i < topLines.size(); i++)
				{
					for (unsigned int j = 0; j < bottomLines.size(); j++)
					{
						if (topLines[i].m_start == bottomLines[j].m_start &&
							topLines[i].m_end == bottomLines[j].m_end)
						{
							wallNormal = topLines[i].m_end - topLines[i].m_start;
							break;
						}
					}
				}
			}

			if (wallNormal.m_x == 0.0f &&
				wallNormal.m_y == 0.0f)
			{
				std::vector<Line> collideLines = collisionLines(collideRect, farthestPoint);

				if (collideLines.size() != 0)
				{
					if (collideLines.size() == 2)
					{
						// Use the center as the normal i guess.
						if (dotProduct(Vector2D(collideLines[0].m_end - collideLines[0].m_start), movement) == 0.0f)
						{
							wallNormal.m_x =
								(collideLines[0].m_end.m_x - collideLines[0].m_start.m_x);
							wallNormal.m_y =
								(collideLines[0].m_end.m_y - collideLines[0].m_start.m_y);
						}
						else if (dotProduct(Vector2D(collideLines[1].m_end - collideLines[1].m_start), movement) == 0.0f)
						{
							wallNormal.m_x =
								(collideLines[1].m_end.m_x - collideLines[1].m_start.m_x);
							wallNormal.m_y =
								(collideLines[1].m_end.m_y - collideLines[1].m_start.m_y);
						}
						else
						{
							wallNormal.m_x = farthestPoint.m_y - collideRect.getCenter().m_y;
							wallNormal.m_y = -(farthestPoint.m_x - collideRect.getCenter().m_x);
						}
					}
					else
					{
						wallNormal.m_x =
							(collideLines[0].m_end.m_x - collideLines[0].m_start.m_x);
						wallNormal.m_y =
							(collideLines[0].m_end.m_y - collideLines[0].m_start.m_y);
					}
				}
			}

			if (wallNormal.m_x != 0.0f ||
				wallNormal.m_y != 0.0f)
			{
				Vector2D leftOverMovement = updatedPosition - box->getPosition();
				Vector2D currentPosition = box->getPosition();

				float dp = dotProduct(leftOverMovement, wallNormal);
				Vector2D slideMove = wallNormal * (dp / (wallNormal.m_x * wallNormal.m_x + wallNormal.m_y * wallNormal.m_y));

				box->setPosition(currentPosition + slideMove);

				wallNormal.m_x = 0.0f;
				wallNormal.m_y = 0.0f;

				if (isnan(box->getPosition().m_x))
				{
					int i = 0;
				}
			}
		}

		// If the first pass didn't solve the problem,
		// Try again with the second rect.
		if (rectIntersectRect(boxRect, collideRect))
		{
			// Look from the other rect's angle.
			std::vector<Vector2D> bClosestPoints = getClosestPoints(collideRect, collideRect.getCenter() - movement);

			// Draw lines and find the furthest collision point.
			Line bFirstRay(bClosestPoints[0], bClosestPoints[0] + moveMod);
			Line bSecondRay(bClosestPoints[1], bClosestPoints[1] + moveMod);

			std::vector<Vector2D> bFirstCol = collisionPoints(boxRect, bFirstRay);
			std::vector<Vector2D> bSecondCol = collisionPoints(boxRect, bSecondRay);

			Vector2D startPoint;
			Vector2D farthestPoint;
			float farthestDist = -1.0f;

			if (bFirstCol.size() != 0)
			{
				startPoint = bFirstRay.m_start;
				farthestPoint = bFirstCol[0];
				farthestDist =
					totalDistance(bFirstRay.m_start, farthestPoint);

				for (unsigned int i = 1; i < bFirstCol.size(); i++)
				{
					if (farthestDist <
						totalDistance(startPoint, bFirstCol[i]))
					{
						farthestPoint = bFirstCol[i];
						farthestDist = totalDistance(startPoint, farthestPoint);
					}
				}
			}

			if (bSecondCol.size() != 0)
			{
				if (farthestDist < 0)
				{
					startPoint = bSecondRay.m_start;
					farthestPoint = bSecondCol[0];
					farthestDist = totalDistance(startPoint, farthestPoint);
				}

				for (unsigned int i = 0; i < bSecondCol.size(); i++)
				{
					if (farthestDist <
						totalDistance(bSecondRay.m_start, bSecondCol[i]))
					{
						farthestPoint = bSecondCol[i];
						startPoint = bSecondRay.m_start;
						farthestDist = totalDistance(startPoint, farthestPoint);
					}
				}
			}

			if (0.0f < farthestDist)
			{
				// Move an extra step out.
				//farthestPoint.m_x = roundf(farthestPoint.m_x);
				//farthestPoint.m_y = roundf(farthestPoint.m_y);

				Vector2D moveAmount = startPoint - farthestPoint + (movement * -0.001f);

				Vector2D rounded = box->getPosition() + moveAmount;

				rounded.m_x = roundThousand(rounded.m_x);
                rounded.m_y = roundThousand(rounded.m_y);

				box->setPosition(rounded);

				if (isnan(box->getPosition().m_x))
				{
					int i = 0;
				}

				Vector2D bRight = collideRect.getBottomRight() - collideRect.getTopRight();
				Vector2D bBottom = collideRect.getBottomLeft() - collideRect.getBottomRight();

				if (dotProduct(bRight, movement) == 0.0f)
				{
					wallNormal = bRight;
				}
				else if (dotProduct(bBottom, movement) == 0.0f)
				{
					wallNormal = bBottom;
				}
				else
				{
					std::vector<Line> collideLines = collisionLines(boxRect, farthestPoint);

					if (collideLines.size() != 0)
					{
						if (collideLines.size() < 2)
						{
							wallNormal.m_x =
								(collideLines[0].m_end.m_x - collideLines[0].m_start.m_x);
							wallNormal.m_y =
								(collideLines[0].m_end.m_y - collideLines[0].m_start.m_y);
						}
					}
				}

				if (wallNormal.m_x != 0.0f ||
					wallNormal.m_y != 0.0f)
				{
					Vector2D leftOverMovement = updatedPosition - box->getPosition();
					Vector2D currentPosition = box->getPosition();

					float dp = dotProduct(leftOverMovement, wallNormal);
					Vector2D slideMove = wallNormal * (dp / (wallNormal.m_x * wallNormal.m_x + wallNormal.m_y * wallNormal.m_y));

					currentPosition += slideMove;

					currentPosition.m_x = roundHundred(currentPosition.m_x);
					currentPosition.m_y = roundHundred(currentPosition.m_y);

					box->setPosition(currentPosition);

					if (isnan(box->getPosition().m_x))
					{
						int i = 0;
					}
				}
			}
		}
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
// Function: vector<Vector2D> getClosestPoints(
// const Rectangle&,
// const Vector2D&) const 
// Description:
// Gets the 2 closest points to the point.
// Parameters:
// const Rectangle& rect - The rect to get the points of.
// const Vector2D& point - The point to get close to.
// Output:
// vector<Vector2D>
// Returns a vector with the two closest points.
//=============================================================================
std::vector<Vector2D> PhysicsSystem::getClosestPoints(
	const Rectangle& rect,
	const Vector2D& point) const
{
	std::vector<Vector2D> points;

	Vector2D topLeft = rect.getTopLeft();
	Vector2D topRight = rect.getTopRight();
	Vector2D bottomLeft = rect.getBottomLeft();
	Vector2D bottomRight = rect.getBottomRight();

	Vector2D smallestPoint;
	Vector2D secondSmallest;

	Vector2D temp[4]{ topLeft, topRight, bottomLeft, bottomRight };

	for (int i = 0; i < 4; i++)
	{
		if (totalDistance(temp[i], point) <
			totalDistance(smallestPoint, point))
		{
			secondSmallest = smallestPoint;
			smallestPoint = temp[i];
		}
		else if(totalDistance(temp[i], point) <
			totalDistance(secondSmallest, point))
		{
			secondSmallest = temp[i];
		}
	}

	points.push_back(smallestPoint);
	points.push_back(secondSmallest);

	return points;
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
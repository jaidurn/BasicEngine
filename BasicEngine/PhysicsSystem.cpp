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
		Vector2D workingMovement = movement;

		if (startPos.m_x == 0.0f && startPos.m_y == 0.0f)
		{
			int i = 0;
		}

		Vector2D endPos = startPos + movement;

		Line distance(startPos, endPos);

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

		std::vector<Vector2D> validMoves;

		std::vector<Vector2D> moveChoices;

		for (unsigned int i = 0; i < movedCollisions.size(); i++)
		{
			// Set up our first move attempt.
			if (movedCollisions[i].m_id != boxID)
			{
				temp = getCollisionBox(movedCollisions[i].m_id);

				if (validMoves.size() == 0)
				{
					moveChoices = moveOutside(box, temp, movement);
				}
				else
				{
					Vector2D lastValidMove = validMoves[validMoves.size() - 1];

					if (lastValidMove.m_x != 0.0f ||
						lastValidMove.m_y != 0.0f)
					{
						box->setPosition(box->getPosition() + lastValidMove);
						moveChoices = moveOutside(box, temp, lastValidMove);
					}
					else
					{
						break;
					}
				}

				if (moveChoices.size() != 0)
				{
					Vector2D oldPosition = box->getPosition();

					if (i < movedCollisions.size() - 1)
					{
						CollisionBox* nextCollision = NULL;

						if (movedCollisions[i + 1].m_id == boxID)
						{
							if(i < movedCollisions.size() - 2)
							{
								nextCollision = getCollisionBox(movedCollisions[i + 2].m_id);
							}
						}
						else
						{
							nextCollision = getCollisionBox(movedCollisions[i + 1].m_id);
						}

						if(nextCollision == NULL)
						{
							break;
						}
						else
						{
							Vector2D closestMove = moveChoices[0];
							float closestDist = -1.0f;

							for (unsigned int j = 0; j < moveChoices.size(); j++)
							{
								box->setPosition(box->getPosition() + moveChoices[j]);
								moveOutside(box, nextCollision, moveChoices[j]);
								
								if (1 < moveChoices.size())
								{
									int hay = 0;
								}

								if (closestDist < 0.0f)
								{
									Vector2D tempClosest = box->getPosition() - startPos;
									
									closestDist = 
										sqrtf(powf(tempClosest.m_x, 2) + powf(tempClosest.m_y, 2));
									closestDist -= sqrtf(powf(movement.m_x, 2) + powf(movement.m_y, 2));

									closestDist = absoluteValue(closestDist);

 									closestMove = moveChoices[j];
								}
								else
								{
									Vector2D tempCompare = box->getPosition() - startPos;

									float compareDist = 
										sqrtf(powf(tempCompare.m_x, 2) + powf(tempCompare.m_y, 2));

									compareDist -= sqrtf(powf(movement.m_x, 2) + powf(movement.m_y, 2));
									float movementDist = sqrtf(powf(movement.m_x, 2) + powf(movement.m_y, 2));

									compareDist = absoluteValue(compareDist);

									if (compareDist < closestDist)
									{
										closestDist = compareDist;
										closestMove = moveChoices[j];
									}
								}

								box->setPosition(oldPosition);
							}

							validMoves.push_back(closestMove);
							moveChoices.clear();
						}
					}
					else
					{
						break;
					}
				}
			}
		}

		// Test our final moves to make sure none of them collide with
		// any thing.
		// lets test the whole thing first
		if (moveChoices.size() != 0)
		{
			Vector2D bestMove = moveChoices[0];
			float bestDist = -1.0f;

			Vector2D oldPosition = box->getPosition();

			for (unsigned int i = 0; i < moveChoices.size(); i++)
			{
				Vector2D maxMove;
				float maxDist = -1.0f;

				box->setPosition(oldPosition + moveChoices[i]);

				std::vector<EntityData> secondPass =
					m_collisionGrid->search(box->getBox());

				for (unsigned int j = 0; j < secondPass.size(); j++)
				{
					if (secondPass[j].m_id != boxID)
					{
						temp = getCollisionBox(secondPass[j].m_id);

						if (temp && temp->getSolid())
						{
							moveOutside(box, temp, moveChoices[i]);

							if (maxDist < 0.0f)
							{
								Vector2D tempMax = box->getPosition() - startPos;
								
								maxDist =
									sqrtf(powf(tempMax.m_x, 2) + powf(tempMax.m_y, 2));

								maxDist -= sqrtf(powf(movement.m_x, 2) + powf(movement.m_y, 2));

								maxDist = absoluteValue(maxDist);

								maxMove = box->getPosition();
							}
							else
							{
								Vector2D tempCompare = box->getPosition() - startPos;

								float compareDist =
									sqrtf(powf(tempCompare.m_x, 2) + powf(tempCompare.m_y, 2));

								compareDist -=
									sqrtf(powf(movement.m_x, 2) + powf(movement.m_y, 2));

								compareDist = absoluteValue(compareDist);

								if (maxDist < compareDist)
								{
									maxDist = compareDist;
									maxMove = box->getPosition();
								}
							}

							box->setPosition(oldPosition);
						}
					}
				}

				if (0.0f < maxDist)
				{
					if (bestDist < 0.0f)
					{
						bestMove = maxMove;
						bestDist = maxDist;
					}
					else
					{
						if (maxDist < bestDist)
						{
							bestMove = maxMove;
							bestDist = maxDist;
						}
					}
				}
			}

			if (0.0f <= bestDist)
			{
				box->setPosition(bestMove);
			}
			else
			{
				box->setPosition(box->getPosition() + bestMove);
			}
		}

		endPos = box->getPosition();

		box->setPosition(startPos);

		m_collisionGrid->removeEntity(EntityData(boxID, box->getBox()));
		
		if (endPos.m_x == 0.0f)
		{
			int i = 0;
		}

		box->setPosition(endPos);

		m_collisionGrid->addEntity(EntityData(boxID, box->getBox()));
	}
}

std::vector<Vector2D> PhysicsSystem::moveToCollision(CollisionBox* box,
	CollisionBox* collision,
	const Vector2D& movement)
{
	std::vector<Vector2D> suggestedMoves;

	if (box && collision)
	{
		Rectangle boxRect = box->getBox();
		Rectangle collideRect = collision->getBox();

		Vector2D boxStartPoint = boxRect.getCenter() - movement;
		Vector2D boxEndPoint = boxRect.getCenter();

		// Make our movement bigger.
		Vector2D moveMod = movement * 10000.0f;
		Vector2D moveNormal = movement / sqrtf(movement.m_x * movement.m_x + movement.m_y * movement.m_y);
		
		// TODO: Test this and make sure it works.
		// It doesn't.
		// Find a way to figure out which plane is the colliding plane
		// and only use that information if we're going away from it.
		if (boxMovingAway(*box, *collision, movement))
		{
			moveMod.m_x *= -1.0f;
			moveNormal.m_x *= -1.0f;

			moveMod.m_y *= -1.0f;
			moveNormal.m_y *= -1.0f;
		}

		// Get the closest points in the movement direction.
		std::vector<Vector2D> boxClosestPoints =
			getClosestPoints(boxRect, boxRect.getCenter() + moveMod);

		Line boxRayA(boxClosestPoints[0], boxClosestPoints[0] - moveMod);
		Line boxRayB(boxClosestPoints[1], boxClosestPoints[1] - moveMod);

		std::vector<Vector2D> boxCollisionsA = 
			collisionPoints(collideRect, boxRayA);

		std::vector<Vector2D> boxCollisionsB = 
			collisionPoints(collideRect, boxRayB);

		std::vector<Vector2D> wallNormals;

		if (boxCollisionsA.size() != 0 ||
			boxCollisionsB.size() != 0)
		{
			Vector2D startPoint;
			Vector2D farthestPoint(-1.0f, -1.0f);
			float farthestDist = -1.0f;

			//Find the farthest point to move back to.
			if (boxCollisionsA.size() != 0)
			{
				startPoint = boxRayA.m_start;

				for (unsigned int i = 0; i < boxCollisionsA.size(); i++)
				{
					if (farthestDist == -1.0f)
					{
						farthestPoint = boxCollisionsA[i];
						farthestDist = totalDistance(startPoint, farthestPoint);
					}
					else
					{
						if (farthestDist <
							totalDistance(startPoint, boxCollisionsA[i]))
						{
							farthestPoint = boxCollisionsA[i];
							farthestDist = totalDistance(startPoint, farthestPoint);
						}
					}
				}
			}

			if (boxCollisionsB.size() != 0)
			{
				for (unsigned int i = 0; i < boxCollisionsB.size(); i++)
				{
					if (farthestDist == -1.0f)
					{
						startPoint = boxRayB.m_start;
						farthestPoint = boxCollisionsB[i];
						farthestDist = totalDistance(startPoint, farthestPoint);
					}
					else
					{
						if (farthestDist <
							totalDistance(startPoint, boxCollisionsB[i]))
						{
							farthestPoint = boxCollisionsB[i];
							farthestDist = totalDistance(startPoint, farthestPoint);
						}
					}
				}
			}

			// Move back to the farthest point.
			Vector2D moveDistance = farthestPoint - startPoint;
			Vector2D correctionAmount =
				moveDistance / sqrtf(moveDistance.m_x * moveDistance.m_x + moveDistance.m_y * moveDistance.m_y);

			moveDistance += moveDistance / 10.0f;

			boxRect.setCenter(boxRect.getCenter() + moveDistance);
			box->setPosition(boxRect.getCenter());

			// Get the current wall normal.
			std::vector<Line> collideLines = 
				collisionLines(collideRect, farthestPoint);

			if (collideLines.size() != 0)
			{
				if (collideLines.size() == 2)
				{
					Vector2D diagonalNormal = farthestPoint - collideRect.getCenter();

					float magnitudes = sqrtf(powf(diagonalNormal.m_x, 2) + powf(diagonalNormal.m_y, 2));
					magnitudes *= sqrtf(powf(moveNormal.m_x, 2) + powf(moveNormal.m_y, 2));
					diagonalNormal =
						diagonalNormal / magnitudes;	
					
					// Check to see if moving diagonally is good.

					if (-1.0f < dotProduct(moveNormal, diagonalNormal))
					{
						float dp = dotProduct(moveNormal, diagonalNormal);

						diagonalNormal.m_x = farthestPoint.m_y - collideRect.getCenter().m_y;
						diagonalNormal.m_y = -(farthestPoint.m_x - collideRect.getCenter().m_x);

						wallNormals.push_back(diagonalNormal);
					}

					wallNormals.push_back(Vector2D(collideLines[0].m_end - collideLines[0].m_start));
					wallNormals.push_back(Vector2D(collideLines[1].m_end - collideLines[1].m_start));
				}
				else
				{
					wallNormals.push_back(Vector2D(collideLines[0].m_end - collideLines[0].m_start));
				}
			}
		}

		// Finish pushing the box outside.
		if (rectIntersectRect(boxRect, collideRect))
		{
			std::vector<Vector2D> collideClosestPoints =
				getClosestPoints(collideRect, collideRect.getCenter() - moveMod);

			Line collideRayA(collideClosestPoints[0], collideClosestPoints[0] + moveMod);
			Line collideRayB(collideClosestPoints[1], collideClosestPoints[1] + moveMod);

			std::vector<Vector2D> collidePointsA = collisionPoints(boxRect, collideRayA);
			std::vector<Vector2D> collidePointsB = collisionPoints(boxRect, collideRayB);

			if (collidePointsA.size() != 0 ||
				collidePointsB.size() != 0)
			{
				Vector2D startPoint;
				Vector2D farthestPoint(-1.0f, -1.0f);
				float farthestDist = -1.0f;

				for (unsigned int i = 0; i < collidePointsA.size(); i++)
				{
					if (farthestDist == -1.0f)
					{
						startPoint = collideRayA.m_start;
						farthestPoint = collidePointsA[i];
						farthestDist = totalDistance(startPoint, farthestPoint);
					}
					else if (farthestDist <
							 totalDistance(startPoint, collidePointsA[i]))
					{
						farthestPoint = collidePointsA[i];
						farthestDist = totalDistance(startPoint, farthestPoint);
					}
				}

				for (unsigned int i = 0; i < collidePointsB.size(); i++)
				{
					if (farthestDist == -1.0f)
					{
						startPoint = collideRayB.m_start;
						farthestPoint = collidePointsB[i];
						farthestDist = totalDistance(startPoint, farthestPoint);
					}
					else if (farthestDist <
							 totalDistance(startPoint, collidePointsB[i]))
					{
						farthestPoint = collidePointsB[i];
						farthestDist = totalDistance(startPoint, farthestPoint);
					}
				}

				Vector2D moveDistance = startPoint - farthestPoint;
				Vector2D correctionAmount =
					moveDistance / sqrtf(moveDistance.m_x * moveDistance.m_x + moveDistance.m_y * moveDistance.m_y);

				moveDistance += correctionAmount / 10.0f;

				boxRect.setCenter(boxRect.getCenter() + moveDistance);
				box->setPosition(boxRect.getCenter());

				// Get the wall normal at this point.
				std::vector<Line> collideLines = collisionLines(collideRect, startPoint);
				
				if (collideLines.size() == 2)
				{
					std::vector<Line> boxCollideLines = collisionLines(boxRect, farthestPoint + moveDistance);

					if (boxCollideLines.size() == 2)
					{
						Vector2D diagonalNormal = boxRect.getCenter() - startPoint;
						float magnitudes = sqrtf(powf(diagonalNormal.m_x, 2) + powf(diagonalNormal.m_y, 2));
						magnitudes *= sqrtf(powf(moveNormal.m_x, 2) + powf(moveNormal.m_y, 2));
						diagonalNormal =
							diagonalNormal / magnitudes;

						// Check to see if moving diagonally is good.
						if (-1.0f < dotProduct(moveNormal, diagonalNormal))
						{
							float dp = dotProduct(moveNormal, diagonalNormal);

							diagonalNormal.m_x = boxRect.getCenter().m_y - startPoint.m_y;
							diagonalNormal.m_y = -(boxRect.getCenter().m_x - startPoint.m_x);

							wallNormals.push_back(diagonalNormal);
						}
					}
					else
					{
						wallNormals.push_back(boxCollideLines[0].m_end - boxCollideLines[0].m_start);
					}

					wallNormals.push_back(Vector2D(collideLines[0].m_end - collideLines[0].m_start));
					wallNormals.push_back(Vector2D(collideLines[1].m_end - collideLines[1].m_start));
				}
				else if (collideLines.size() != 0)
				{
					wallNormals.push_back(Vector2D(collideLines[0].m_end - collideLines[0].m_start));
				}
			}
		}

		// Project the remaining movement along the wall normal.
		if (wallNormals.size() != 0)
		{
			Vector2D prevPosition = box->getPosition();
			Vector2D leftOverMovement = boxEndPoint - prevPosition;

			for (unsigned int i = 0; i < wallNormals.size(); i++)
			{
				float dp = dotProduct(leftOverMovement, wallNormals[i]);

				Vector2D moveProposal(wallNormals[i] * (dp / (powf(wallNormals[i].m_x, 2) + powf(wallNormals[i].m_y, 2))));

				box->setPosition(prevPosition + moveProposal);
			
				if (!rectIntersectRect(box->getBox(), collideRect))
				{
					suggestedMoves.push_back(moveProposal);
				}

				box->setPosition(prevPosition);
			}
		}
	}

	return suggestedMoves;
}

//=============================================================================
// Function: void moveOutside(CollisionBox*,
// CollisionBox*,
// Vector2D&)
// Description:
// Moves the box outside of the box it's colliding with.
// Parameters:
// CollisionBox* box - The box to move.
// CollisionBox* collision - The box it's inside.
// Vector2D& movement - The movement being attempted.
//=============================================================================
std::vector<Vector2D> PhysicsSystem::moveOutside(CollisionBox* box,
	CollisionBox* collision,
	const Vector2D& movement)
{
	std::vector<Vector2D> suggestedMoves;
	std::vector<Vector2D> wallNormals;

	Rectangle boxRect = box->getBox();
	Rectangle collideRect = collision->getBox();

	Vector2D moveNormal = movement / sqrtf(powf(movement.m_x, 2) + powf(movement.m_y, 2));
	Vector2D moveMod = moveNormal * (boxRect.getWidth() / 2);

	Vector2D endGoal = boxRect.getCenter();

	std::vector<Vector2D> boxClosestPoints = getClosestPoints(boxRect, collideRect.getCenter());

	// Check to see if any of the points are inside the collide box.
	for (unsigned int i = 0; i < boxClosestPoints.size(); i++)
	{
		// If we find a point inside the collide box, find the lines it collides with.
		if (pointInRect(collideRect, boxClosestPoints[i]))
		{
			std::vector<Line> collideLines;

			std::vector<Line> boxPointLines = getPointLines(boxRect, boxClosestPoints[i]);

			if (boxPointLines.size() != 0)
			{
				for (unsigned int j = 0; j < boxPointLines.size(); j++)
				{
					// Find the lines the lines collide with.
					float slope = 0.0f;
					float lengthMod = 100.0f;
					float lineLength = totalDistance(boxPointLines[j]);
					Line longerLine = boxPointLines[j];

					if (boxClosestPoints[i] == longerLine.m_end)
					{
						longerLine.m_end = longerLine.m_start;
						longerLine.m_start = boxPointLines[j].m_end;
					}

					// Create a long version of the lines.
					longerLine.m_end.m_x =
						longerLine.m_end.m_x + (longerLine.m_end.m_x - longerLine.m_start.m_x) / lineLength * lengthMod;
					longerLine.m_end.m_y =
						longerLine.m_end.m_y + (longerLine.m_end.m_y - longerLine.m_start.m_y) / lineLength * lengthMod;

					collisionLines(collideLines, collideRect, longerLine);
				}

				if (collideLines.size() != 0)
				{
					Line moveLine(boxClosestPoints[i], boxClosestPoints[i] - moveMod);

					Vector2D moveAmount(0.0f, 0.0f);

					bool collides = false;

					for (unsigned int j = 0; j < collideLines.size(); j++)
					{
						if (linesIntersect(collideLines[j], moveLine))
						{
							collides = true;

							break;
						}
					}

					if (!collides)
					{
						moveLine.m_end = moveLine.m_start + moveMod;
					}

					if (collideLines.size() == 2)
					{
						if (linesIntersect(collideLines[0], moveLine))
						{
							if (!linesIntersect(collideLines[1], moveLine))
							{
								moveAmount = intersectPoint(collideLines[0], moveLine) - moveLine.m_start;
								wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);
								wallNormals.push_back(collideLines[1].m_end - collideLines[1].m_start);
							}
							else
							{
								float distLineA = totalDistance(intersectPoint(collideLines[0], moveLine), moveLine.m_start);
								float distLineB = totalDistance(intersectPoint(collideLines[0], moveLine), moveLine.m_start);

								if(distLineA < distLineB)
								{
									moveAmount = intersectPoint(collideLines[0], moveLine) - moveLine.m_start;
									wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);
								}
								else if (distLineA == distLineB)
								{
									moveAmount = intersectPoint(collideLines[0], moveLine) - moveLine.m_start;

									wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);
									wallNormals.push_back(collideLines[1].m_end - collideLines[1].m_start);
									wallNormals.push_back(collideRect.getCenter() - intersectPoint(collideLines[0], moveLine));
								}
								else
								{
									moveAmount = intersectPoint(collideLines[1], moveLine) - moveLine.m_start;
									wallNormals.push_back(collideLines[1].m_end - collideLines[1].m_start);
								}

							}
						}
						else if (linesIntersect(collideLines[1], moveLine))
						{
							moveAmount = intersectPoint(collideLines[1], moveLine) - moveLine.m_start;

							wallNormals.push_back(collideLines[1].m_end - collideLines[1].m_start);
							wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);
						}
						else
						{
							int yee = 0;
						}
					}
					else 
					{
						if (linesIntersect(collideLines[0], moveLine))
						{
							moveAmount = intersectPoint(collideLines[0], moveLine) - moveLine.m_start;

							wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);
						}
						else
						{
							moveAmount = boxClosestPoints[i] - closestPointToPoint(collideLines[0], boxClosestPoints[i]);
							wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);
						}
					}

					if (moveAmount.m_x != 0.0f ||
						moveAmount.m_y != 0.0f)
					{
						std::vector<Line> normalCollides = collisionLines(collideRect, boxClosestPoints[i] + moveAmount);

						for (unsigned int k = 0; k < normalCollides.size(); k++)
						{
							if (k == 1)
							{
								int ayy = 0;
							}

							wallNormals.push_back(normalCollides[k].m_end - normalCollides[k].m_start);
						}

						Vector2D moveExtra = moveAmount / sqrtf(powf(moveAmount.m_x, 2) + powf(moveAmount.m_y, 2));

						moveExtra = moveExtra / 2.0f;

						clamp(moveExtra.m_x, -1.0f, 1.0f);
						clamp(moveExtra.m_y, -1.0f, 1.0f);
						
						if (5.0f <= moveAmount.m_x ||
							5.0f <= moveAmount.m_y)
						{
							int what = 0;
						}

						boxRect.setCenter(boxRect.getCenter() + moveAmount + moveExtra);
						break;
					}
				}
			}
		}
	}

	if (rectIntersectRect(boxRect, collideRect))
	{
		std::vector<Vector2D> collideClosePoints = getClosestPoints(collideRect, boxRect.getCenter());

		if (collideClosePoints.size() != 0)
		{
			for (unsigned int i = 0; i < collideClosePoints.size(); i++)
			{
				if (pointInRect(boxRect, collideClosePoints[i]))
				{
					std::vector<Line> collideLines;

					std::vector<Line> collidePointLines = getPointLines(collideRect, collideClosePoints[i]);

					if (collidePointLines.size() != 0)
					{
						for (unsigned int j = 0; j < collidePointLines.size(); j++)
						{
							// Find the lines the lines collide with.
							float slope = 0.0f;
							float lengthMod = 100.0f;
							float lineLength = totalDistance(collidePointLines[j]);
							Line longerLine = collidePointLines[j];

							if (collideClosePoints[i] == longerLine.m_end)
							{
								longerLine.m_end = longerLine.m_start;
								longerLine.m_start = collidePointLines[j].m_end;
							}

							// Create a long version of the lines.
							longerLine.m_end.m_x =
								longerLine.m_end.m_x + (longerLine.m_end.m_x - longerLine.m_start.m_x) / lineLength * lengthMod;
							longerLine.m_end.m_y =
								longerLine.m_end.m_y + (longerLine.m_end.m_y - longerLine.m_start.m_y) / lineLength * lengthMod;

							collisionLines(collideLines, boxRect, longerLine);
						}
					}

					if (collideLines.size() != 0)
					{
						Line moveLine(collideClosePoints[i], collideClosePoints[i] - moveMod);

						Vector2D moveAmount(0.0f, 0.0f);

						bool collides = false;

						for (unsigned int j = 0; j < collideLines.size(); j++)
						{
							if (linesIntersect(collideLines[j], moveLine))
							{
								collides = true;

								break;
							}
						}

						if (!collides)
						{
							moveLine.m_end = moveLine.m_start + moveMod;
						}

						if (collideLines.size() == 2)
						{
							if (linesIntersect(collideLines[0], moveLine))
							{
								if (!linesIntersect(collideLines[1], moveLine))
								{
									moveAmount = intersectPoint(collideLines[0], moveLine) - moveLine.m_start;
									wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);

									if (5.0f <= moveAmount.m_x ||
										5.0f <= moveAmount.m_y)
									{
										int what = 0;
									}
								}
								else
								{
									float distLineA = totalDistance(intersectPoint(collideLines[0], moveLine), moveLine.m_start);
									float distLineB = totalDistance(intersectPoint(collideLines[0], moveLine), moveLine.m_start);

									if (distLineA < distLineB)
									{
										moveAmount = intersectPoint(collideLines[0], moveLine) - moveLine.m_start;
										wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);

										if (5.0f <= moveAmount.m_x ||
											5.0f <= moveAmount.m_y)
										{
											int what = 0;
										}
									}
									else if (distLineA == distLineB)
									{
										moveAmount = intersectPoint(collideLines[0], moveLine) - moveLine.m_start;

										wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);
										wallNormals.push_back(collideLines[1].m_end - collideLines[1].m_start);
										wallNormals.push_back(collideRect.getCenter() - intersectPoint(collideLines[0], moveLine));

										if (5.0f <= moveAmount.m_x ||
											5.0f <= moveAmount.m_y)
										{
											int what = 0;
										}
									}
									else
									{
										moveAmount = intersectPoint(collideLines[1], moveLine) - moveLine.m_start;
										wallNormals.push_back(collideLines[1].m_end - collideLines[1].m_start);

										if (5.0f <= moveAmount.m_x ||
											5.0f <= moveAmount.m_y)
										{
											int what = 0;
										}
									}
								}
							}
							else if (linesIntersect(collideLines[1], moveLine))
							{
								Vector2D intersection = intersectPoint(collideLines[1], moveLine);

								moveAmount = intersectPoint(collideLines[1], moveLine) - moveLine.m_start;
								wallNormals.push_back(collideLines[1].m_end - collideLines[1].m_start);

								if (5.0f <= moveAmount.m_x ||
									5.0f <= moveAmount.m_y)
								{
									int what = 0;
								}
							}
							else
							{
								int haw = 0;
							}

							if (5.0f <= moveAmount.m_x ||
								5.0f <= moveAmount.m_y)
							{
								int what = 0;
							}
						}
						else
						{
							Vector2D closestPoint = closestPointToPoint(collideLines[0], collideClosePoints[i]);

							if (totalDistance(closestPoint, collideRect.getCenter()) <
								totalDistance(collideClosePoints[i], collideRect.getCenter()))
							{
								moveAmount = collideClosePoints[i] - closestPoint;
								wallNormals.push_back(collideLines[0].m_end - collideLines[0].m_start);
								wallNormals.push_back(collidePointLines[0].m_end - collidePointLines[0].m_start);
								wallNormals.push_back(collidePointLines[1].m_end - collidePointLines[1].m_start);
							}

							if (5.0f <= moveAmount.m_x ||
								5.0f <= moveAmount.m_y)
							{
								int what = 0;
							}
						}

						if (moveAmount.m_x != 0.0f ||
							moveAmount.m_y != 0.0f)
						{
							Vector2D moveExtra = moveAmount / sqrtf(powf(moveAmount.m_x, 2) + powf(moveAmount.m_y, 2));

							moveExtra = moveExtra / 2.0f;

							clamp(moveExtra.m_x, -1.0f, 1.0f);
							clamp(moveExtra.m_y, -1.0f, 1.0f);

							if (5.0f <= moveAmount.m_x ||
								5.0f <= moveAmount.m_y)
							{
								int what = 0;
							}

							boxRect.setCenter(boxRect.getCenter() + moveAmount + moveExtra);
							break;
						}
					}
				}
			}
		}
	}

	box->setPosition(boxRect.getCenter());

	if (wallNormals.size() != 0)
	{
		Vector2D prevPosition = boxRect.getCenter();
		Vector2D leftOverMovement = endGoal - prevPosition;

		for (unsigned int i = 0; i < wallNormals.size(); i++)
		{
			float dp = dotProduct(leftOverMovement, wallNormals[i]);

			Vector2D moveProposal(wallNormals[i] * (dp / (powf(wallNormals[i].m_x, 2) + powf(wallNormals[i].m_y, 2))));

			if (abs(moveProposal.m_x) < 0.005f)
			{
				moveProposal.m_x = 0.0f;
			}

			if (abs(moveProposal.m_y) < 0.005f)
			{
				moveProposal.m_y = 0.0f;
			}

			if (moveProposal.m_x != 0.0f ||
				moveProposal.m_y != 0.0f)
			{
				Vector2D moveModifier;

				moveModifier.m_x = wallNormals[i].m_y;
				moveModifier.m_y = -wallNormals[i].m_x;

				moveModifier = moveModifier / sqrtf(powf(moveModifier.m_x, 2) + powf(moveModifier.m_y, 2));

				moveModifier = moveModifier / 2.0f;

				boxRect.setCenter(prevPosition + moveProposal + moveModifier);

				if (!rectIntersectRect(boxRect, collideRect))
				{
					suggestedMoves.push_back(moveProposal);
				}
			}
		}
	}

	return suggestedMoves;
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

	Vector2D smallestPoint = topLeft;
	Vector2D secondSmallest = topRight;

	Vector2D temp[4]{ topLeft, topRight, bottomLeft, bottomRight };

	for (int i = 1; i < 4; i++)
	{
		if (totalDistance(temp[i], point) <
			totalDistance(smallestPoint, point))
		{
			secondSmallest = smallestPoint;
			smallestPoint = temp[i];
		}
		else if (totalDistance(temp[i], point) <
			totalDistance(secondSmallest, point))
		{
			secondSmallest = temp[i];
		}
	}

	points.push_back(smallestPoint);
	points.push_back(secondSmallest);

	return points;
}

bool PhysicsSystem::boxMovingAway(const CollisionBox& box,
	const CollisionBox& collide,
	const Vector2D& movement)
{
	Rectangle boxRect = box.getBox();
	boxRect.setCenter(boxRect.getCenter() - movement);

	Rectangle collideRect = collide.getBox();

	Vector2D moveMod = movement * 10000.0f;

	std::vector<Vector2D> boxClosestPoints = getClosestPoints(boxRect, collideRect.getCenter());

	// Check to see if any of the points are inside the collide box.
	for (unsigned int i = 0; i < boxClosestPoints.size(); i++)
	{
		// If we find a point inside the collide box, find the lines it collides with.
		if (pointInRect(collideRect, boxClosestPoints[i]))
		{
			std::vector<Line> collideLines;

			std::vector<Line> boxPointLines = getPointLines(boxRect, boxClosestPoints[i]);

			if (boxPointLines.size() != 0)
			{
				for (unsigned int j = 0; j < boxPointLines.size(); j++)
				{
					// Find the lines the lines collide with.
					float slope = 0.0f;
					float lengthMod = 100.0f;
					float lineLength = totalDistance(boxPointLines[j]);
					Line longerLine = boxPointLines[j];

					if (boxClosestPoints[i] == longerLine.m_end)
					{
						longerLine.m_end = longerLine.m_start;
						longerLine.m_start = boxPointLines[j].m_end;
					}

					// Create a long version of the lines.
					longerLine.m_end.m_x =
						longerLine.m_end.m_x + (longerLine.m_end.m_x - longerLine.m_start.m_x) / lineLength * lengthMod;
					longerLine.m_end.m_y =
						longerLine.m_end.m_y + (longerLine.m_end.m_y - longerLine.m_start.m_y) / lineLength * lengthMod;

					collisionLines(collideLines, collideRect, longerLine);
				}

				if (collideLines.size() != 0)
				{
					Line moveLine(boxClosestPoints[i], boxClosestPoints[i] + moveMod);

					Vector2D planeNormal(0.0f, 0.0f);

					if (collideLines.size() == 2)
					{
						if (linesIntersect(collideLines[0], moveLine))
						{
							if (!linesIntersect(collideLines[1], moveLine))
							{
								planeNormal.m_x = -(collideLines[0].m_end.m_y - collideLines[0].m_start.m_y);
								planeNormal.m_y = collideLines[0].m_end.m_x - collideLines[0].m_start.m_x;
							}
							else
							{
								float yDistance = sqrtf(powf(moveLine.m_start.m_y - boxRect.getCenter().m_y, 2));
								float xDistance = sqrtf(powf(moveLine.m_start.m_x - boxRect.getCenter().m_x, 2));

								if (xDistance <
									yDistance)
								{
									planeNormal.m_x = -(collideLines[0].m_end.m_y - collideLines[0].m_start.m_y);
									planeNormal.m_y = collideLines[0].m_end.m_x - collideLines[0].m_start.m_x;
								}
								else
								{
									planeNormal.m_x = -(collideLines[1].m_end.m_y - collideLines[1].m_start.m_y);
									planeNormal.m_y = collideLines[1].m_end.m_x - collideLines[1].m_start.m_x;
								}
							}
						}
						else if (linesIntersect(collideLines[1], moveLine))
						{
							planeNormal.m_x = -(collideLines[1].m_end.m_y - collideLines[1].m_start.m_y);
							planeNormal.m_y = collideLines[1].m_end.m_x - collideLines[1].m_start.m_x;
						}
					}
					else if (collideLines.size() == 1)
					{
						if (linesIntersect(collideLines[0], moveLine))
						{
							planeNormal.m_x = -(collideLines[0].m_end.m_y - collideLines[0].m_start.m_y);
							planeNormal.m_y = collideLines[0].m_end.m_x - collideLines[0].m_start.m_x;
						}
					}

					if (planeNormal.m_x != 0.0f ||
						planeNormal.m_y != 0.0f)
					{
						Vector2D planeNormalized = planeNormal / sqrtf(powf(planeNormal.m_x, 2) + powf(planeNormal.m_y, 2));
						Vector2D moveNormalized = movement / sqrtf(powf(movement.m_x, 2) + powf(movement.m_y, 2));

						if (dotProduct(planeNormalized, moveNormalized) < 0.0f)
						{
							float dp = dotProduct(planeNormalized, moveNormalized);
							return true;
						}
					}
				}
			}
		}
	}

	std::vector<Vector2D> collideClosePoints = getClosestPoints(collideRect, boxRect.getCenter());

	if (collideClosePoints.size() != 0)

		for (unsigned int i = 0; i < collideClosePoints.size(); i++)
		{
			if (pointInRect(boxRect, collideClosePoints[i]))
			{
				std::vector<Line> collideLines;

				std::vector<Line> collidePointLines = getPointLines(collideRect, collideClosePoints[i]);

				if (collidePointLines.size() != 0)
				{
					for (unsigned int j = 0; j < collidePointLines.size(); j++)
					{
						// Find the lines the lines collide with.
						float slope = 0.0f;
						float lengthMod = 100.0f;
						float lineLength = totalDistance(collidePointLines[j]);
						Line longerLine = collidePointLines[j];

						if (collideClosePoints[i] == longerLine.m_end)
						{
							longerLine.m_end = longerLine.m_start;
							longerLine.m_start = collidePointLines[j].m_end;
						}

						// Create a long version of the lines.
						longerLine.m_end.m_x =
							longerLine.m_end.m_x + (longerLine.m_end.m_x - longerLine.m_start.m_x) / lineLength * lengthMod;
						longerLine.m_end.m_y =
							longerLine.m_end.m_y + (longerLine.m_end.m_y - longerLine.m_start.m_y) / lineLength * lengthMod;

						collisionLines(collideLines, boxRect, longerLine);
					}
				}

				if (collideLines.size() != 0)
				{
					Line moveLine(collideClosePoints[i], collideClosePoints[i] - moveMod);

					Vector2D planeNormal(0.0f, 0.0f);

					if (collideLines.size() == 2)
					{
						if (linesIntersect(collideLines[0], moveLine))
						{
							if (!linesIntersect(collideLines[1], moveLine))
							{
								planeNormal.m_x = -(collideLines[0].m_end.m_y - collideLines[0].m_start.m_y);
								planeNormal.m_y = collideLines[0].m_end.m_x - collideLines[0].m_start.m_x;
							}
							else
							{
								float yDistance = sqrtf(powf(moveLine.m_start.m_y - collideRect.getCenter().m_y, 2));
								float xDistance = sqrtf(powf(moveLine.m_start.m_x - collideRect.getCenter().m_x, 2));

								if (xDistance <
									yDistance)
								{
									planeNormal.m_x = -(collideLines[0].m_end.m_y - collideLines[0].m_start.m_y);
									planeNormal.m_y = collideLines[0].m_end.m_x - collideLines[0].m_start.m_x;
								}
								else
								{
									planeNormal.m_x = -(collideLines[1].m_end.m_y - collideLines[1].m_start.m_y);
									planeNormal.m_y = collideLines[1].m_end.m_x - collideLines[1].m_start.m_y;
								}
							}
						}
						else if (linesIntersect(collideLines[1], moveLine))
						{
							planeNormal.m_x = -(collideLines[1].m_end.m_y - collideLines[1].m_start.m_y);
							planeNormal.m_y = collideLines[1].m_end.m_x - collideLines[1].m_start.m_x;
						}
					}
					else if (collideLines.size() == 1)
					{
						if (linesIntersect(collideLines[0], moveLine))
						{
							planeNormal.m_x = -(collideLines[0].m_end.m_y - collideLines[0].m_start.m_y);
							planeNormal.m_y = collideLines[0].m_end.m_x - collideLines[0].m_start.m_x;
						}
					}

					if (planeNormal.m_x != 0.0f ||
						planeNormal.m_y != 0.0f)
					{
						Vector2D planeNormalized = planeNormal / sqrtf(powf(planeNormal.m_x, 2) + powf(planeNormal.m_y, 2));
						Vector2D moveNormalized = movement / sqrtf(powf(movement.m_x, 2) + powf(movement.m_y, 2));

						if (dotProduct(planeNormalized, moveNormalized) < 0.0f)
						{
							float dp = dotProduct(planeNormalized, moveNormalized);
							return true;
						}
					}
				}
			}
		}

	return false;
}

std::vector<Line> PhysicsSystem::getPointLines(const Rectangle& rect,
	const Vector2D& point)
{
	std::vector<Line> lines;

	Line top(rect.getTopLeft(), rect.getTopRight());
	Line right(rect.getTopRight(), rect.getBottomRight());
	Line bottom(rect.getBottomRight(), rect.getBottomLeft());
	Line left(rect.getBottomLeft(), rect.getTopLeft());

	if (top.m_start == point || top.m_end == point)
	{
		lines.push_back(top);
	}

	if (right.m_start == point || right.m_end == point)
	{
		lines.push_back(right);
	}

	if (bottom.m_start == point || bottom.m_end == point)
	{
		lines.push_back(bottom);
	}

	if (left.m_start == point || left.m_end == point)
	{
		lines.push_back(left);
	}

	return lines;
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
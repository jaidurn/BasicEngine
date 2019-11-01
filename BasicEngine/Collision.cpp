#include "Collision.h"
#include "Rotation.h"
#include "BBMath.h"
#include <vector>

//=============================================================================
// Function: bool pointInRect(const Rectangle&, const Vector2D&)
// Description:
// Checks to see if a point is inside the rect.
// It does this by rotating the point around the rect using a
// rotation that's negative to the rects rotation.
// This allows for a simpler check.
// Parameters:
// const Rectangle& rect - The rect to check.
// const Vector2D& point - The point to check.
// Output:
// bool
// Returns true if the point is inside the rect.
// Returns false if the point is outside the rect.
//=============================================================================
bool pointInRect(const Rectangle& rect,
	const Vector2D& point)
{
	bool inside = false;

	Vector2D rotatedPoint = rotatePoint(rect.getCenter(), point, -rect.getRotation());

	float leftX = rect.getCenter().m_x - ((float)rect.getWidth() / 2.0f);
	float rightX = rect.getCenter().m_x + ((float)rect.getWidth() / 2.0f);
	float topY = rect.getCenter().m_y - ((float)rect.getHeight() / 2.0f);
	float bottomY = rect.getCenter().m_y + ((float)rect.getHeight() / 2.0f);

	if (leftX <= rotatedPoint.m_x &&
		rotatedPoint.m_x <= rightX &&
		topY <= rotatedPoint.m_y &&
		rotatedPoint.m_y <= bottomY)
	{
		inside = true;
	}

	return inside;
}

//=============================================================================
// Function: bool lineInRect(const Rectangle&, const Line&)
// Description:
// Checks to see if a line intersects with a rectangle.
// Parameters:
// const Rectangle& rect - The rect to check.
// const Line& line - The line to check.
// Output:
// bool
// Returns true if line is in rectangle.
// Returns false if the line isn't in the rect.
//=============================================================================
bool lineInRect(const Rectangle& rect,
	const Line& line)
{
	bool inside = false;

	Line top(rect.getTopLeft(), rect.getTopRight());
	Line bottom(rect.getBottomLeft(), rect.getBottomRight());
	Line right(rect.getTopRight(), rect.getBottomRight());
	Line left(rect.getTopLeft(), rect.getBottomLeft());

	if (pointInRect(rect, line.m_start) &&
		pointInRect(rect, line.m_end))
	{
		inside = true;
	}
	else if (linesIntersect(line, top) ||
		linesIntersect(line, bottom) ||
		linesIntersect(line, right) ||
		linesIntersect(line, left))
	{
		inside = true;
	}

	return inside;
}

//=============================================================================
// Function: bool rectInsideRect(const Rectangle&, const Rectangle&)
// Description:
// Checks to see if a rect is completely inside another rect.
// Parameters:
// const Rectangle& a - The rect to check if it's inside.
// const Rectangle& b - The rect it's inside.
// Output:
// bool
// Returns true if the rectangle is inside.
// Returns false if the rectangle isn't inside.
//=============================================================================
bool rectInsideRect(const Rectangle& a, const Rectangle& b)
{
	bool inside = false;

	if (pointInRect(b, a.getTopLeft()) &&
		pointInRect(b, a.getTopRight()) &&
		pointInRect(b, a.getBottomLeft()) &&
		pointInRect(b, a.getBottomRight()))
	{
		inside = true;
	}

	return inside;
}

//=============================================================================
// Function: bool rectIntersectRect(const Rectangle&, const Rectangle&)
// Description:
// Checks to see if two rectangles are colliding or not.
// Parameters:
// const Rectangle& a - The first rect to check.
// const Rectangle& b - The second rect to check.
// Output:
// bool
// Returns true if the rectangles collide.
// Returns false if the rectangles don't collide.
//=============================================================================
bool rectIntersectRect(const Rectangle& a,
	const Rectangle& b)
{
	bool inside = true;

	if (a.getRotation() == 0.0f &&
		b.getRotation() == 0.0f)
	{
		Vector2D aTopLeft = a.getTopLeft();
		Vector2D aBottomRight = a.getBottomRight();

		Vector2D bTopLeft = b.getTopLeft();
		Vector2D bBottomRight = b.getBottomRight();

		if (aBottomRight.m_x < bTopLeft.m_x ||
			bBottomRight.m_x < aTopLeft.m_x ||
			aBottomRight.m_y < bTopLeft.m_y ||
			bBottomRight.m_y < aTopLeft.m_y)
		{
			inside = false;
		}
	}
	else
	{
		inside = false;

		// Find the closest corners
		const int CORNER_COUNT = 4;

		Vector2D aCorners[CORNER_COUNT]{ a.getTopLeft(),
		a.getTopRight(),
		a.getBottomRight(),
		a.getBottomLeft() };

		Vector2D bCorners[CORNER_COUNT]{ b.getTopLeft(),
		b.getTopRight(),
		b.getBottomRight(),
		b.getBottomLeft() };

		int aClosestIndex = 0;
		int bClosestIndex = 0;

		float aDistance = 
			totalDistance(aCorners[aClosestIndex], b.getCenter());
		float bDistance =
			totalDistance(bCorners[aClosestIndex], a.getCenter());

		for (int i = 0; i < CORNER_COUNT; i++)
		{
			if (totalDistance(aCorners[i], b.getCenter()) < aDistance)
			{
				aDistance = totalDistance(aCorners[i], b.getCenter());
				aClosestIndex = i;
			}

			if (totalDistance(bCorners[i], a.getCenter()) < bDistance)
			{
				bDistance = totalDistance(bCorners[i], a.getCenter());
				bClosestIndex = i;
			}
		}
		
		int aNextIndex = aClosestIndex + 1;
		int aPrevIndex = aClosestIndex - 1;

		int bNextIndex = bClosestIndex + 1;
		int bPrevIndex = bClosestIndex - 1;

		if (aPrevIndex < 0)
		{
			aPrevIndex = CORNER_COUNT - 1;
		}

		if (CORNER_COUNT <= aNextIndex)
		{
			aNextIndex = 0;
		}

		if (bPrevIndex < 0)
		{
			bPrevIndex = CORNER_COUNT - 1;
		}
		
		if (CORNER_COUNT <= bNextIndex)
		{
			bNextIndex = 0;
		}

		Line aPrev(aCorners[aClosestIndex], aCorners[aPrevIndex]);
		Line aNext(aCorners[aClosestIndex], aCorners[aNextIndex]);
		
		Line bPrev(bCorners[bClosestIndex], bCorners[bPrevIndex]);
		Line bNext(bCorners[bClosestIndex], bCorners[bNextIndex]);

		if (pointInRect(b, aCorners[aClosestIndex]) ||
			pointInRect(a, bCorners[bClosestIndex]))
		{
			inside = true;
		}
		else 
		{
			if (linesIntersect(aPrev, bPrev) ||
				linesIntersect(aNext, bPrev) ||
				linesIntersect(aPrev, bNext) ||
				linesIntersect(aNext, bNext))
			{
				inside = true;
			}
		}
	}

	return inside;
}

//=============================================================================
// Function: Vector2D intersectPoint(const Rectangle&, const Line&)
// Description:
// Gets the intersection point between a rect and a line.
// Parameters:
// const Rectangle& rect - The rect to check.
// const Line& line - The line to check.
// Output:
// Vector2D
// Returns the intersection point between the line and the rectangle.
// Returns a negative vector if there isn't an intersection.
//=============================================================================
Vector2D intersectPoint(const Rectangle& rect, const Line& line)
{
	if (lineInRect(rect, line))
	{
		Vector2D closestPoint(0.0f, 0.0f);

		Line top(rect.getTopLeft(), rect.getTopRight());
		Line bottom(rect.getBottomLeft(), rect.getBottomRight());
		Line right(rect.getTopRight(), rect.getBottomRight());
		Line left(rect.getTopLeft(), rect.getBottomLeft());

		if (linesIntersect(line, top))
		{
			closestPoint = intersectPoint(line, top);
		}

		if (linesIntersect(line, bottom))
		{
			Vector2D otherPoint = intersectPoint(line, bottom);

			if (totalDistance(line.m_start, otherPoint) <
				totalDistance(line.m_start, closestPoint))
			{
				closestPoint = otherPoint;
			}
		}

		if (linesIntersect(line, left))
		{
			Vector2D otherPoint = intersectPoint(line, left);

			if (totalDistance(line.m_start, otherPoint) <
				totalDistance(line.m_start, closestPoint))
			{
				closestPoint = otherPoint;
			}
		}

		if (linesIntersect(line, right))
		{
			Vector2D otherPoint = intersectPoint(line, right);

			if (totalDistance(line.m_start, otherPoint) <
				totalDistance(line.m_start, closestPoint))
			{
				closestPoint = otherPoint;
			}
		}

		return closestPoint;
	}

	return Vector2D(-1.0f, -1.0f);
}

//=============================================================================
// Function: Vector2D intersectPoint(const Rectangle&,
// const Rectangle&)
// Description:
// Gets the intersection point between two rectangles.
// Parameters:
// const Rectangle& a - The first rect to check.
// const Rectangle& b - The second rect to check.
// Output:
// Vector2D
// Returns the collision point if there's a collision.
// Returns a negative vector if there isn't.
//=============================================================================
Vector2D intersectPoint(const Rectangle& a,
	const Rectangle& b)
{
	Vector2D point(-1.0f, -1.0f);

	if (rectIntersectRect(a, b))
	{
		Line aTop(a.getTopLeft(), a.getTopRight());
		Line aRight(a.getTopRight(), a.getBottomRight());
		Line aBottom(a.getBottomLeft(), a.getBottomRight());
		Line aLeft(a.getTopLeft(), a.getBottomLeft());

		Vector2D bCenter = b.getCenter();

		Line closestLine = aTop;
		float closestDistance = distanceFromPoint(closestLine, bCenter);

		if (distanceFromPoint(aRight, bCenter) <
			closestDistance)
		{
			closestLine = aRight;
			closestDistance = distanceFromPoint(closestLine, bCenter);
		}

		if (distanceFromPoint(aBottom, bCenter) <
			closestDistance)
		{
			closestLine = aBottom;
			closestDistance = distanceFromPoint(closestLine, bCenter);
		}

		if (distanceFromPoint(aLeft, bCenter) <
			closestDistance)
		{
			closestLine = aLeft;
			closestDistance = distanceFromPoint(closestLine, bCenter);
		}

		Vector2D closestPoint = closestPointToPoint(closestLine, bCenter);

		if (closestPoint == closestLine.m_start ||
			closestPoint == closestLine.m_end)
		{
			point = closestPoint;
		}
		else
		{
			float xPos = 0.0f;
			float yPos = 0.0f;

			if (a.getCenter().m_x <= b.getCenter().m_x)
			{
				fMax(xPos, closestLine.m_start.m_x, closestLine.m_end.m_x);
			}
			else
			{
				fMin(xPos, closestLine.m_start.m_x, closestLine.m_end.m_x);
			}

			if (a.getCenter().m_y <= b.getCenter().m_y)
			{
				fMax(yPos, closestLine.m_start.m_y, closestLine.m_end.m_y);
			}
			else
			{
				fMin(yPos, closestLine.m_start.m_y, closestLine.m_end.m_y);
			}

			point.m_x = xPos;
			point.m_y = yPos;
		}
	}

	return point;
}

//=============================================================================
// Function: Vector2D overlapAmount(const Rectangle& a,
// const Rectangle& b)
// Description:
// Gets the amount of overlap between two rectangles.
// Parameters:
// const Rectangle& a - The first rectangle to check.
// const Rectangle& b - The second rectangle to check.
// Output:
// Vector2D
// Returns a vector with the amount of x overlap and y overlap.
// If no overlap, returns a negative vector.
//=============================================================================
Vector2D overlapAmount(const Rectangle& a,
	const Rectangle& b)
{
	float xOverlap = -1.0f;
	float yOverlap = -1.0f;

	if (rectIntersectRect(a, b))
	{
		Line aTop(a.getTopLeft(), a.getTopRight());
		Line aRight(a.getTopRight(), a.getBottomRight());
		Line aBottom(a.getBottomLeft(), a.getBottomRight());
		Line aLeft(a.getTopLeft(), a.getBottomLeft());

		Vector2D aCenter = a.getCenter();
		Vector2D bCenter = b.getCenter();

		Line closestLine = aTop;
		float closestDistance = distanceFromPoint(closestLine, bCenter);

		if (distanceFromPoint(aRight, bCenter) <
			closestDistance)
		{
			closestLine = aRight;
			closestDistance = distanceFromPoint(closestLine, bCenter);
		}

		if (distanceFromPoint(aBottom, bCenter) <
			closestDistance)
		{
			closestLine = aBottom;
			closestDistance = distanceFromPoint(closestLine, bCenter);
		}

		if (distanceFromPoint(aLeft, bCenter) <
			closestDistance)
		{
			closestLine = aLeft;
			closestDistance = distanceFromPoint(closestLine, bCenter);
		}

		Vector2D closestPoint = closestPointToPoint(closestLine, bCenter);

		Line bTop(b.getTopLeft(), b.getTopRight());
		Line bRight(b.getTopRight(), b.getBottomRight());
		Line bBottom(b.getBottomLeft(), b.getBottomRight());
		Line bLeft(b.getTopLeft(), b.getBottomLeft());

		float bX = b.getMaxX();

		if (aCenter.m_x <= bCenter.m_x)
		{
			bX = b.getMinX();

		}

		Line bFirstX(Vector2D(-1.0f, -1.0f), Vector2D(-1.0f, -1.0f));
		Line bSecondX(Vector2D(-1.0f, -1.0f), Vector2D(-1.0f, -1.0f));

		if (bTop.m_start.m_x == bX ||
			bTop.m_end.m_x == bX)
		{
			bFirstX = bTop;
		}

		if (bRight.m_start.m_x == bX ||
			bRight.m_end.m_x == bX)
		{
			if (bFirstX.m_start.m_x == -1.0f)
			{
				bFirstX = bRight;
			}
			else
			{
				bSecondX = bRight;
			}
		}

		if (bBottom.m_start.m_x == bX ||
			bBottom.m_end.m_x == bX)
		{
			if (bFirstX.m_start.m_x == -1.0f)
			{
				bFirstX = bBottom;
			}
			else
			{
				bSecondX = bBottom;
			}
		}

		if (bLeft.m_start.m_x == bX ||
			bLeft.m_end.m_x == bX)
		{
			bSecondX = bLeft;
		}

		Line closeLineStartToBX(closestLine.m_start, Vector2D(bX, closestLine.m_start.m_y));
		Line closeLineEndToBX(closestLine.m_end, Vector2D(bX, closestLine.m_end.m_y));

		if (totalDistance(closeLineStartToBX) < totalDistance(closeLineEndToBX))
		{
			xOverlap = (closeLineEndToBX.m_end.m_x - closeLineEndToBX.m_start.m_x);
		}
		else
		{
			xOverlap = (closeLineStartToBX.m_end.m_x - closeLineStartToBX.m_start.m_x);
		}

		float bY = b.getMaxY();

		if (aCenter.m_y <= bCenter.m_y)
		{
			bY = b.getMinY();

		}

		Line bFirstY(Vector2D(-1.0f, -1.0f), Vector2D(-1.0f, -1.0f));
		Line bSecondY(Vector2D(-1.0f, -1.0f), Vector2D(-1.0f, -1.0f));

		if (bTop.m_start.m_y == bY ||
			bTop.m_end.m_y == bY)
		{
			bFirstY = bTop;
		}

		if (bRight.m_start.m_y == bY ||
			bRight.m_end.m_y == bY)
		{
			if (bFirstY.m_start.m_y == -1.0f)
			{
				bFirstY = bRight;
			}
			else
			{
				bSecondY = bRight;
			}
		}

		if (bBottom.m_start.m_y == bY ||
			bBottom.m_end.m_y == bY)
		{
			if (bFirstY.m_start.m_y == -1.0f)
			{
				bFirstY = bBottom;
			}
			else
			{
				bSecondY = bBottom;
			}
		}

		if (bLeft.m_start.m_y == bY ||
			bLeft.m_end.m_y == bY)
		{
			bSecondY = bLeft;
		}

		Line closeLineStartToBY(closestLine.m_start, Vector2D(closestLine.m_start.m_x, bY));
		Line closeLineEndToBY(closestLine.m_end, Vector2D(closestLine.m_end.m_x, bY));

		if (totalDistance(closeLineStartToBY) < totalDistance(closeLineEndToBY))
		{
			yOverlap = (closeLineEndToBY.m_end.m_y - closeLineEndToBY.m_start.m_y);
		}
		else
		{
			yOverlap = (closeLineStartToBY.m_end.m_y - closeLineStartToBY.m_start.m_y);
		}
	}

	return Vector2D(xOverlap, yOverlap);
}
#include "Collision.h"
#include "Rotation.h"

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

	if (linesIntersect(line, top) ||
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
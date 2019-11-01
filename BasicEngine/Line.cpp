#include "Line.h"
#include <cmath>

//=============================================================================
// Function: bool linesIntersect(const Line, const Line)
// Description:
// Checks to see if two lines intersect.
// Parameters:
// const Line a - The first line.
// const Line b - The second line.
// Output:
// bool
// Returns true if the lines intersect.
// Returns false if the lines don't intersect.
//=============================================================================
bool linesIntersect(const Line a, const Line b)
{
	bool intersect = false;
	
	if(isUndefined(a) && isUndefined(b))
	{
		// Make sure they have the same x
		if (a.m_start.m_x == b.m_start.m_x)
		{
			Vector2D intersection = intersectPoint(a, b);

			// Make sure they intersect
			if (pointOnLine(a, intersection) && 
				pointOnLine(b, intersection))
			{
				intersect = true;
			}
		}
	}
	else
	{
		if (isUndefined(a) || isUndefined(b))
		{
			Vector2D intersection = intersectPoint(a, b);

			if (pointOnLine(a, intersection) &&
				pointOnLine(b, intersection))
			{
				intersect = true;
			}
		}
		else
		{
			float slopeA = getSlope(a);
			float slopeB = getSlope(b);

			// If the lines have the same slope
			if (slopeA == slopeB)
			{
				float interceptA = getIntercept(a);
				float interceptB = getIntercept(b);
				// And they aren't parallel
				if (interceptA == interceptB)
				{
					// Make sure they actually touch.
					if (pointOnLine(a, b.m_start) || pointOnLine(a, b.m_end))
					{
						intersect = true;
					}
				}
			}
			else
			{
				Vector2D intersection = intersectPoint(a, b);

				if (pointOnLine(a, intersection) && pointOnLine(b, intersection))
				{
					intersect = true;
				}
			}
		}
	}
	
	return intersect;
}

//=============================================================================
// Function: Vector2D intersectPoint(const Line, const Line)
// Description:
// Gets the intersection point of two lines.
// Parameters:
// const Line a - The first line.
// const Line b - The second line.
// Output:
// Vector2D
// Returns a Vector containing the intersection points.
//=============================================================================
Vector2D intersectPoint(const Line a, const Line b)
{
	Vector2D point;

	point.m_x = -1;
	point.m_y = -1;

	// If both lines are undefined.
	if (isUndefined(a) && isUndefined(b))
	{
		if (a.m_start.m_x == b.m_start.m_x)
		{
			if (pointOnLine(b, a.m_start))
			{
				point = a.m_start;
			}
			else if (pointOnLine(b, a.m_end))
			{
				point = a.m_end;
			}
			else if (pointOnLine(a, b.m_start))
			{
				point = b.m_start;
			}
			else if (pointOnLine(a, b.m_end))
			{
				point = b.m_end;
			}
		}
	}
	else
	{
		if (isUndefined(a))
		{
			float slopeB = getSlope(b);
			float intersectB = getIntercept(b);

			float collideX = a.m_start.m_x;
			float collideY = (slopeB * a.m_start.m_x) + intersectB;

			point.m_x = collideX;
			point.m_y = collideY;
		}
		else if (isUndefined(b))
		{
			float slopeA = getSlope(a);
			float intersectA = getIntercept(a);

			float collideX = b.m_start.m_x;
			float collideY = (slopeA * b.m_start.m_x) + intersectA;

			point.m_x = collideX;
			point.m_y = collideY;
		}
		else
		{
			float slopeA = getSlope(a);
			float slopeB = getSlope(b);

			float intersectA = getIntercept(a);
			float intersectB = getIntercept(b);

			float collideX = ((slopeA * a.m_start.m_x) - (slopeB * b.m_start.m_x) + (b.m_start.m_y - a.m_start.m_y)) / (slopeA - slopeB);
			float collideY = (slopeA * (collideX - a.m_start.m_x) + a.m_start.m_y);

			collideX = collideX;
			collideY = collideY;

			point.m_x = collideX;
			point.m_y = collideY;
		}
	}

	return point;
}

//=============================================================================
// Function: float getSlope(const Line)
// Description:
// Calculates the slope of a line.
// Parameters:
// const Line line - The line to get the slope.
// Output:
// float
// Returns the slope.
//=============================================================================
float getSlope(const Line line)
{
	float slope = 0;

	float xDiff = line.m_end.m_x - line.m_start.m_x;
	float yDiff = line.m_end.m_y - line.m_start.m_y;

	if (xDiff != 0)
	{
		slope = yDiff / xDiff;
	}

	return slope;
}

//=============================================================================
// Function: float getIntercept(const Line)
// Description:
// Gets the Y intercept of a Line.
// Parameters:
// const Line line - The line to get the y intercept for.
// Output:
// float
// Returns the y intercept of the line.
//=============================================================================
float getIntercept(const Line line)
{
	float intercept = -1;
	float slope = getSlope(line);

	intercept = line.m_start.m_y - (line.m_start.m_x * slope);

	return intercept;
}

//=============================================================================
// Function: bool isUndefined(const Line)
// Description:
// Checks to see if a lines slope is undefined.
// Parameters:
// const Line line - The line to check.
// Output:
// bool
// Returns true if the slope is undefined.
// Returns false if the slope is defined.
//=============================================================================
bool isUndefined(const Line line)
{
	bool undefined = false;

	if (line.m_end.m_x == line.m_start.m_x)
	{
		undefined = true;
	}

	return undefined;
}

//=============================================================================
// Function: bool pointOnLine(const Line, const Vector2D)
// Description:
// Checks to see if the provided point is on the line.
// Parameters:
// const Line line - The line to use.
// const Vector2D point - The point to check.
// Output:
// bool
// Returns true if the point is on the line.
// Returns false if the point isn't on the line.
//=============================================================================
bool pointOnLine(const Line line, const Vector2D point)
{
	bool onLine = false;

	float startDistance = totalDistance(line.m_start, point);
	float endDistance = totalDistance(line.m_end, point);

	float realDist = totalDistance(line);
	float startAndEnd = startDistance + endDistance;

	if (startDistance + endDistance == totalDistance(line))
	{
		onLine = true;
	}

	return onLine;
}

//=============================================================================
// Function: Vector2D getMidPoint(const Line&)
// Description:
// Gets the midpoint of a line segment.
// Parameters:
// const Line& line - The line to get the mid point of.
// Output:
// Vector2D
// Returns the midpoint position.
//=============================================================================
Vector2D getMidPoint(const Line& line)
{
	float xPoint = line.m_start.m_x + line.m_end.m_x;
	float yPoint = line.m_start.m_y + line.m_end.m_y;

	xPoint /= 2.0f;
	yPoint /= 2.0f;

	return Vector2D(xPoint, yPoint);
}

//=============================================================================
// Function: float distanceFromPoint(const Line&,
// const Vector2D&)
// Description:
// Gets the distance between a line and a point.
// Parameters:
// const Line& line - The line to use.
// const Vector2D& point - The point to get the distance of.
// Output:
// float
// Returns the distance between the point and the line.
// Returns -1 on error.
//=============================================================================
float distanceFromPoint(const Line& line,
	const Vector2D& point)
{
	return totalDistance(point, closestPointToPoint(line, point));
}

//=============================================================================
// Function: Vector2D closestPointToPoint(const Line&,
// const Vector2D&)
// Description:
// Gets the closest point on the line segment to the point.
// Parameters:
// const Line& line - The line to use.
// const Vector2D& point - The point to get close to.
// Output:
// Vector2D
// Returns the closest point.
// Returns a negative vector on failure.
//=============================================================================
Vector2D closestPointToPoint(const Line& line,
	const Vector2D& point)
{
	float a = line.m_end.m_y - line.m_start.m_y;
	float b = line.m_end.m_x - line.m_start.m_x;
	float c = line.m_end.m_x * line.m_start.m_y - line.m_start.m_x * line.m_end.m_y;

	float xPoint = -1.0f;
	float yPoint = -1.0f;

	if (a == 0.0f && b != 0.0f)
	{
		xPoint = point.m_x;
		yPoint = -c / b;
	}
	else if (a != 0.0f && b == 0.0f)
	{
		xPoint = -c / a;
		yPoint = point.m_y;

	}
	else if (a != 0.0f && b != 0.0f)
	{
		xPoint = b * (b * point.m_x - a * point.m_y) - a * c;
		xPoint /= (a * a) + (b * b);

		yPoint = a * (-b * point.m_x + a * point.m_y) - b * c;
		yPoint /= (a * a) + (b * b);
	}

	if (xPoint != -1.0f && yPoint != -1.0f)
	{
		Vector2D closestPoint(xPoint, yPoint);

		if (!pointOnLine(line, closestPoint))
		{
			if (totalDistance(line.m_start, closestPoint) <
				totalDistance(line.m_end, closestPoint))
			{
				xPoint = line.m_start.m_x;
				yPoint = line.m_start.m_y;
			}
			else
			{
				xPoint = line.m_end.m_x;
				yPoint = line.m_end.m_y;
			}
		}
	}

	return Vector2D(xPoint, yPoint);
}

//=============================================================================
// Function: float totalDistance(const Line)
// Description:
// Calculates the distance between two points.
// Parameters:
// const Line line - The line to calculate the distance for.
// Output:
// float
// Returns the total distance.
//=============================================================================
float totalDistance(const Line line)
{
	float distance = -1;

	float xDiff = line.m_end.m_x - line.m_start.m_x;
	float yDiff = line.m_end.m_y - line.m_start.m_y;

	xDiff *= xDiff;
	yDiff *= yDiff;

	distance = xDiff + yDiff;

	return sqrt(distance);
}

//=============================================================================
// Function: float totalDistance(const Vector2D, const Vector2D)
// Description:
// Calculates the distance between two points.
// Parameters:
// const Vector2D start - The starting point.
// const Vector2D end - The ending point.
// Output:
// float
// Returns the distance between two points.
//=============================================================================
float totalDistance(const Vector2D start, const Vector2D end)
{
	Line line(start, end);

	return totalDistance(line);
}
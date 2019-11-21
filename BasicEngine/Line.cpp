#include "Line.h"
#include <cmath>
#include "BBMath.h"

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

	Line workingA = a;
	Line workingB = b;
	
	if(isUndefined(workingA) && isUndefined(workingB))
	{
		// Make sure they have the same x
		if (workingA.m_start.m_x == workingB.m_start.m_x)
		{
			Vector2D intersection = intersectPoint(workingA, workingB);

			// Make sure they intersect
			if (pointOnLine(workingA, intersection) && 
				pointOnLine(workingB, intersection))
			{
				intersect = true;
			}
		}
	}
	else
	{
		if (isUndefined(workingA) || isUndefined(workingB))
		{
			Vector2D intersection = intersectPoint(workingA, workingB);

			if (pointOnLine(workingA, intersection) &&
				pointOnLine(workingB, intersection))
			{
				intersect = true;
			}
		}
		else
		{
			float slopeA = getSlope(workingA);
			float slopeB = getSlope(workingB);

			// If the lines have the same slope
			if (slopeA == slopeB)
			{
				float interceptA = getIntercept(workingA);
				float interceptB = getIntercept(workingB);
				// And they aren't parallel
				if (interceptA == interceptB)
				{
					// Make sure they actually touch.
					if (pointOnLine(workingA, workingB.m_start) || pointOnLine(workingA, workingB.m_end))
					{
						intersect = true;
					}
				}
			}
			else
			{
				Vector2D intersection = intersectPoint(workingA, workingB);

				if (pointOnLine(workingA, intersection) && pointOnLine(workingB, intersection))
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

	Line workingA = a;
	Line workingB = b;

	// If both lines are undefined.
	if (isUndefined(workingA) && isUndefined(workingB))
	{
		if (workingA.m_start.m_x == workingB.m_start.m_x)
		{
			if (pointOnLine(workingB, workingA.m_start))
			{
				point = workingA.m_start;
			}
			else if (pointOnLine(workingB, workingA.m_end))
			{
				point = workingA.m_end;
			}
			else if (pointOnLine(workingA, workingB.m_start))
			{
				point = workingB.m_start;
			}
			else if (pointOnLine(workingA, workingB.m_end))
			{
				point = workingB.m_end;
			}
		}
	}
	else
	{
		if (isUndefined(workingA))
		{
			float slopeB = getSlope(workingB);
			float intersectB = getIntercept(workingB);

			float collideX = workingA.m_start.m_x;
			float collideY = (slopeB * workingA.m_start.m_x) + intersectB;

			point.m_x = collideX;
			point.m_y = collideY;

			if (isnan(point.m_x) || isnan(point.m_y))
			{
				int i = 0;
			}
		}
		else if (isUndefined(workingB))
		{
			float slopeA = getSlope(workingA);
			float intersectA = getIntercept(workingA);

			float collideX = workingB.m_start.m_x;
			float collideY = (slopeA * workingB.m_start.m_x) + intersectA;

			point.m_x = collideX;
			point.m_y = collideY;

			if (isnan(point.m_x) || isnan(point.m_y))
			{
				int i = 0;
			}
		}
		else
		{
			float slopeA = getSlope(workingA);
			float slopeB = getSlope(workingB);

			float intersectA = getIntercept(workingA);
			float intersectB = getIntercept(workingB);

			if (slopeA - slopeB == 0.0)
			{
				if (intersectA == intersectB)
				{
					if (pointOnLine(workingB, workingA.m_start))
					{
						point = workingA.m_start;
					}
					else if (pointOnLine(workingB, workingA.m_end))
					{
						point = workingA.m_end;
					}
					else if (pointOnLine(workingA, workingB.m_start))
					{
						point = workingB.m_start;
					}
					else if (pointOnLine(workingA, workingB.m_end))
					{
						point = workingB.m_end;
					}
					else
					{
						point = workingB.m_start;
					}
				}
			}
			else
			{
				float collideX = ((slopeA * workingA.m_start.m_x) - (slopeB * workingB.m_start.m_x) + (workingB.m_start.m_y - workingA.m_start.m_y)) / (slopeA - slopeB);
				float collideY = (slopeA * (collideX - workingA.m_start.m_x) + workingA.m_start.m_y);

				collideX = collideX;
				collideY = collideY;

				point.m_x = collideX;
				point.m_y = collideY;
			}
			if (isnan(point.m_x) || isnan(point.m_y))
			{
				int i = 0;
			}
		}
	}

	if (isnan(point.m_x) || isnan(point.m_y))
	{
		int i = 0;
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

	intercept = roundf(line.m_start.m_y - (line.m_start.m_x * slope));

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

	Line workingLine = line;
	Vector2D workingPoint = point;

	float startDistance = totalDistance(workingLine.m_start, workingPoint);
	float endDistance = totalDistance(workingLine.m_end, workingPoint);

	float realDist = totalDistance(workingLine);
	float startAndEnd = startDistance + endDistance;

	if (abs(startAndEnd - realDist) < EPSILON)
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

//=============================================================================
// Function: void roundLine(Line&)
// Description:
// Rounds the points of the line to be in pixel space.
// Parameters:
// Line& line - The line to round.
//=============================================================================
void roundLine(Line& line)
{
	line.m_start.m_x = roundf(line.m_start.m_x);
	line.m_start.m_y = roundf(line.m_start.m_y);
	line.m_end.m_x = roundf(line.m_end.m_x);
	line.m_end.m_y = roundf(line.m_end.m_y);
}
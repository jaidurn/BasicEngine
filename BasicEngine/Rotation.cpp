#include "Rotation.h"
#include <cmath>

//=============================================================================
// Function: float radiansToDegrees(const float)
// Description:
// Converts radians to degrees.
// Parameters:
// const float radians - The radian measurement to convert.
// Output:
// float
// Returns the degrees.
//=============================================================================
float radiansToDegrees(const float radians)
{
	float degrees = 0.0f;

	float tempRadians = radians;

	tempRadians /= (2.0f * (float)_PI);

	degrees = tempRadians * _CIRCLE_DEGREES;

	return degrees;
}

//=============================================================================
// Function: float degreesToRadians(const float)
// Description:
// Converts degrees to radians.
// Parameters:
// const float degrees - The degrees to convert to radians.
// Output:
// float
// Returns the degrees in radians.
//=============================================================================
float degreesToRadians(const float degrees)
{
	float radians = 0.0f;

	float tempDegrees = degrees;

	tempDegrees /= _CIRCLE_DEGREES;

	tempDegrees *= (2.0f * (float)_PI);

	radians = tempDegrees;

	return radians;
}

//=============================================================================
// Function: float percentToDegrees(const float)
// Description:
// Converts a percentage into degrees.
// Parameters:
// const float percent - The percentage to convert to degrees.
// Output:
// float
// Returns the percentage in degrees.
//=============================================================================
float percentToDegrees(const float percent)
{
	float degrees = _CIRCLE_DEGREES * (percent / 100.0f);
	
	return degrees;
}

//=============================================================================
// Function: float angleToDegrees(const Vector2D&, const Vector2D&)
// Description:
// Calculates the angle between the point and the origin.
// Unlike atan2, this angle starts from the positive Y axis.
// Parameters:
// const Vector2D& origin - The origin point.
// const Vector2D& point - The point to get the angle of.
// Output:
// float
// Returns the angle in degrees.
//=============================================================================
float angleToDegrees(const Vector2D& origin,
	const Vector2D& point)
{
	float degrees = 0.0f;

	float xDiff = point.m_x - origin.m_x;
	float yDiff = point.m_y - origin.m_y;

	degrees = radiansToDegrees(atan2(xDiff, yDiff));

	if (degrees < 0)
	{
		degrees += _CIRCLE_DEGREES;
	}

	return degrees;
}

//=============================================================================
// Function: Vector2D rotatePoint(const Vector2D&, 
//								  const Vector2D&,
//								  const float)
// Description:
// Rotates a point around the origin by the specified amount.
// Parameters:
// const Vector2D& origin - The origin point to rotate around.
// const Vector2D& point - The point to rotate.
// Output:
// Vector2D
// Returns a vector containing the rotated point.
//=============================================================================
Vector2D rotatePoint(const Vector2D& origin,
	const Vector2D& point,
	const float percent)
{
	float tempX = point.m_x - origin.m_x;
	float tempY = point.m_y - origin.m_y;

	float degrees = percentToDegrees(percent);
	float radians = degreesToRadians(degrees);

	float radSine = sin(radians);
	float radCosine = cos(radians);

	float rotatedX = round((tempX * radCosine) - (tempY * radSine));
	float rotatedY = round((tempX * radSine) + (tempY * radCosine));

	return Vector2D(origin.m_x + rotatedX, origin.m_y + rotatedY);
}
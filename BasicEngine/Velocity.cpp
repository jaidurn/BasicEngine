#include "Velocity.h"

Velocity::Velocity()
	:m_direction(0.0f, 0.0f)
{

}

Velocity::~Velocity()
{

}

//=============================================================================
// Function: const Vector2D& getDirection() const
// Description:
// Gets the direction of the vector.
// Output:
// const Vector2D&
// Returns the current direction.
//=============================================================================
const Vector2D& Velocity::getDirection() const
{
	return m_direction;
}

//=============================================================================
// Function: const Vector2D getNormal() const
// Description:
// Gets the normalized vector of the velocity.
// Output:
// const Vector2D
// Returns the normalized vector.
//=============================================================================
const Vector2D Velocity::getNormal() const
{
	float x = m_direction.m_x / getMagnitude();
	float y = m_direction.m_y / getMagnitude();

	return Vector2D(x, y);
}

//=============================================================================
// Function: const float getMagnitude() const
// Description:
// Gets the current magnitude of the velocity.
// Output:
// const float 
// Returns the magnitude of the velocity.
//=============================================================================
const float Velocity::getMagnitude() const
{
	return totalDistance(Vector2D(0.0f, 0.0f), m_direction);
}

//=============================================================================
// Function: void addVelocity(const Vector2D&)
// Description:
// Adds velocity to the current vector.
// Parameters:
// const Vector2D& direction - The velocity to add.
//=============================================================================
void Velocity::addVelocity(const Vector2D& direction)
{
	m_direction += direction;
}
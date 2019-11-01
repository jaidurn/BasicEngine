#include "Vector2D.h"

Vector2D operator+(const Vector2D& a, const Vector2D& b)
{
	return Vector2D(a.m_x + b.m_x, a.m_y + b.m_y);
}

Vector2D operator-(const Vector2D& a, const Vector2D& b)
{
	return Vector2D(a.m_x - b.m_x, a.m_y - b.m_y);
}

Vector2D operator*(const Vector2D& a, const Vector2D& b)
{
	return Vector2D(a.m_x * b.m_x, a.m_y * b.m_y);
}

Vector2D operator/(const Vector2D& a, const Vector2D& b)
{
	return Vector2D(a.m_x / b.m_x, a.m_y / b.m_y);
}

//=============================================================================
// Function: const int dotProduct(const Vector2D&, const Vector2D&)
// Description:
// Gets the dot product of two vectors.
// Parameters:
// const Vector2D& a - The first vector to compare.
// const Vector2D& b - The second vector to compare.
// Output:
// const int
// Returns a positive number if they're facing the same way.
// Returns 0 if they're not facing the same way.
// Returns a negative if they're facing opposite ways.
//=============================================================================
const int dotProduct(const Vector2D& a, const Vector2D& b)
{
	return a.m_x * b.m_x + a.m_y * b.m_y;
}
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

Vector2D operator*(const Vector2D& point, const float amount)
{
	return Vector2D(point.m_x * amount, point.m_y * amount);
}

Vector2D operator/(const Vector2D& a, const Vector2D& b)
{
	return Vector2D(a.m_x / b.m_x, a.m_y / b.m_y);
}

Vector2D operator/(const Vector2D& point, const float amount)
{
	return Vector2D(point.m_x / amount, point.m_y / amount);
}

bool operator==(const Vector2D& a, const Vector2D& b)
{
	float xDiff = a.m_x - b.m_x;
	float yDiff = a.m_y - b.m_y;

	if (xDiff < 0.0f)
	{
		xDiff *= -1;
	}
	
	if (yDiff < 0.0f)
	{
		yDiff *= -1;
	}

	return(xDiff <= 0.02 && yDiff <= 0.02);
}

bool operator!=(const Vector2D& a, const Vector2D& b)
{
	return(a.m_x != b.m_x || a.m_y != b.m_y);
}

//=============================================================================
// Function: BB_fstream& operator<<(BB_fstream&, const Vector2D&) 
// Description: 
// Reads the contents of the vector out to the BB file stream.
// Parameters: 
// BB_fstream& bbstream - The BB file stream to read to.
// const Vector2D& vector - The vector to read out.
// Output: 
// BB_fstream&
// Returns the modified BB_fstream.
//=============================================================================
BB_fstream& operator<<(BB_fstream& bbstream,
	const Vector2D& vector)
{
	if (bbstream.isOpen())
	{
		bbstream << vector.m_x;
		bbstream << vector.m_y;
	}

	return bbstream;
}

//=============================================================================
// Function: BB_fstream& operator>>(BB_fstream&, Vector2D&)
// Description: 
// Reads vector information in from the BB filestream.
// Parameters: 
// BB_fstream& bbstream - The BB file stream to read from.
// Vector2D& vector - The vector to read into.
// Output: 
// BB_fstream&
// Returns the modified file stream.
//=============================================================================
BB_fstream& operator>>(BB_fstream& bbstream,
	Vector2D& vector)
{
	if (bbstream.isOpen())
	{
		bbstream >> vector.m_x;
		bbstream >> vector.m_y;
	}

	return bbstream;
}

//=============================================================================
// Function: const float dotProduct(const Vector2D&, const Vector2D&)
// Description:
// Gets the dot product of two vectors.
// Parameters:
// const Vector2D& a - The first vector to compare.
// const Vector2D& b - The second vector to compare.
// Output:
// const float
// Returns a positive number if they're facing the same way.
// Returns 0 if they're not facing the same way.
// Returns a negative if they're facing opposite ways.
//=============================================================================
const float dotProduct(const Vector2D& a, const Vector2D& b)
{
	float product = a.m_x * b.m_x + a.m_y * b.m_y;

	return product;
}
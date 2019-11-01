#pragma once
//==========================================================================================
// File Name: Vector2D.h
// Author: Brian Blackmon
// Date Created: 10/16/2019
// Purpose: 
// Allows for storage of x and y coordinates.
//==========================================================================================

struct Vector2D
{
	Vector2D()
		:m_x(0), m_y(0)
	{

	}

	Vector2D(float x, float y)
		:m_x(x), m_y(y)
	{

	}

	float m_x;
	float m_y;

	Vector2D& operator=(const Vector2D& vector)
	{
		this->m_x = vector.m_x;
		this->m_y = vector.m_y;

		return *this;
	}

	void operator+=(const Vector2D& vector)
	{
		this->m_x = this->m_x + vector.m_x;
		this->m_y = this->m_y + vector.m_y;
	}

	void operator-=(const Vector2D& vector)
	{
		this->m_x = this->m_x - vector.m_x;
		this->m_y = this->m_y - vector.m_y;
	}

	void operator*=(const Vector2D& vector)
	{
		this->m_x = this->m_x * vector.m_x;
		this->m_y = this->m_y * vector.m_y;
	}

	void operator/=(const Vector2D& vector)
	{
		this->m_x = this->m_x / vector.m_x;
		this->m_y = this->m_y / vector.m_y;
	}

};

Vector2D operator+(const Vector2D& a, const Vector2D& b);
Vector2D operator-(const Vector2D& a, const Vector2D& b);
Vector2D operator*(const Vector2D& a, const Vector2D& b);
Vector2D operator/(const Vector2D& a, const Vector2D& b);

bool operator==(const Vector2D& a, const Vector2D& b);
bool operator!=(const Vector2D& a, const Vector2D& b);

const int dotProduct(const Vector2D& a, const Vector2D& b);
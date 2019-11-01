#pragma once
//==========================================================================================
// File Name: Velocity.h
// Author: Brian Blackmon
// Date Created: 10/30/2019
// Purpose: 
// Holds information about velocity.
//==========================================================================================
#include "Vector2D.h"
#include "Line.h"

class Velocity
{
public:
	Velocity();
	~Velocity();

	const Vector2D& getDirection() const;
	const Vector2D getNormal() const;
	const float getMagnitude() const;

	void addVelocity(const Vector2D& direction);

private:
	Vector2D m_direction;
};


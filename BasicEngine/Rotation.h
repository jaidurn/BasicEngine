#pragma once
//==========================================================================================
// File Name: Rotation.h
// Author: Brian Blackmon
// Date Created: 10/18/2019
// Purpose: 
// Holds functions for rotating points around a point.
//==========================================================================================
#include "Vector2D.h"

#define _PI 3.14159
#define _CIRCLE_DEGREES 360

float radiansToDegrees(const float radians);
float degreesToRadians(const float degrees);
float percentToDegrees(const float percent);
float angleToDegrees(const Vector2D& origin,
	const Vector2D& point);

Vector2D rotatePoint(const Vector2D& origin,
	const Vector2D& point,
	const float percent);
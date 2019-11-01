#pragma once
//==========================================================================================
// File Name: Collision.h
// Author: Brian Blackmon
// Date Created: 10/18/2019
// Purpose: 
// Provides a series of collision functions for checking
// and handling collisions between objects.
//==========================================================================================
#include "Rectangle.h"
#include "Line.h"

bool pointInRect(const Rectangle& rect,
	const Vector2D& point);

bool lineInRect(const Rectangle& rect,
	const Line& line);

bool rectInsideRect(const Rectangle& a,
	const Rectangle& b);

bool rectIntersectRect(const Rectangle& a,
	const Rectangle& b);

Vector2D intersectPoint(const Rectangle& rect,
	const Line& line);

Vector2D intersectPoint(const Rectangle& a,
	const Rectangle& b);

Vector2D overlapAmount(const Rectangle& a,
	const Rectangle& b);
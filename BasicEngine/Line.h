#pragma once
//==========================================================================================
// File Name: Line.h
// Author: Brian Blackmon
// Date Created: 10/16/2019
// Purpose: 
// Contains the line struct and line related functions.
//==========================================================================================
#include "Vector2D.h"

struct Line
{
	Line(Vector2D start, Vector2D end)
		:m_start(start), m_end(end)
	{
	}

	Vector2D m_start;
	Vector2D m_end;
};

bool linesIntersect(const Line a, const Line b);
Vector2D intersectPoint(const Line a, const Line b);

float getSlope(const Line line);
float getIntercept(const Line line);
bool isUndefined(const Line line);

bool pointOnLine(const Line line, const Vector2D point);

Vector2D getMidPoint(const Line& line);

float distanceFromPoint(const Line& line, const Vector2D& point);
Vector2D closestPointToPoint(const Line& line, const Vector2D& point);

float totalDistance(const Line line);
float totalDistance(const Vector2D start, const Vector2D end);

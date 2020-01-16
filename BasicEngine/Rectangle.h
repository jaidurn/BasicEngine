#pragma once
//==========================================================================================
// File Name: Rectangle.h
// Author: Brian Blackmon
// Date Created: 10/16/2019
// Purpose: 
// Holds rectangle information, such as the center location,
// rotation information, and corner location.
//==========================================================================================
#include "Vector2D.h"
#include "BB_fstream.h"

class Rectangle
{
public:
	Rectangle();
	Rectangle(const Vector2D& center,
		const int width,
		const int height);
	Rectangle(const Rectangle& rect);
	~Rectangle();

	Rectangle& operator=(const Rectangle& rect);

	const Vector2D& getCenter() const;
	const Vector2D getTopLeft() const;
	const Vector2D getTopRight() const;
	const Vector2D getBottomLeft() const;
	const Vector2D getBottomRight() const;
	const int getWidth() const;
	const int getHeight() const;
	const float getRotation() const;

	const float getMinX() const;
	const float getMinY() const;
	const float getMaxX() const;
	const float getMaxY() const;

	void setCenter(const float x, const float y);
	void setCenter(const Vector2D& center);
	void setWidth(const int width);
	void setHeight(const int height);
	void setRotation(const float rotation);

private:
	Vector2D m_center;

	int m_width;
	int m_height;

	float m_rotation;
};

BB_fstream& operator<<(BB_fstream& bbstream,
	const Rectangle& rect);

BB_fstream& operator>>(BB_fstream& bbstream,
	Rectangle& rect);
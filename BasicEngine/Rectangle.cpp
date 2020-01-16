#include "Rectangle.h"
#include "Rotation.h"
#include <cmath>

Rectangle::Rectangle()
	:m_center(0.0f, 0.0f),
	m_width(0),
	m_height(0),
	m_rotation(0.0f)
{

}

Rectangle::Rectangle(const Vector2D& center,
	const int width,
	const int height)
	:m_width(width), 
	m_height(height),
	m_rotation(0.0f)
{
	m_center.m_x = center.m_x;
	m_center.m_y = center.m_y;
}

Rectangle::Rectangle(const Rectangle& rect)
	:m_width(rect.getWidth()),
	m_height(rect.getHeight()),
	m_rotation(rect.getRotation())
{
	m_center = rect.getCenter();
}

Rectangle::~Rectangle()
{
}

Rectangle& Rectangle::operator=(const Rectangle& rect)
{
	if (&rect == this)
	{
		return *this;
	}

	m_center = rect.getCenter();
	m_width = rect.m_width;
	m_height = rect.m_height;
	m_rotation = rect.m_rotation;

	return *this;
}

//=============================================================================
// Function: const Vector2D& getCenter() const
// Description:
// Gets the center coordinates of the rectangle.
// Output:
// const Vector2D&
// Returns the center coordinates.
//=============================================================================
const Vector2D& Rectangle::getCenter() const
{
	return m_center;
}

//=============================================================================
// Function: const Vector2D getTopLeft() const
// Description:
// Gets the coordinates of the top left corner.
// Output:
// const Vector2D
// Returns a vector containing the coordinates of the top left point.
//=============================================================================
const Vector2D Rectangle::getTopLeft() const
{
	Vector2D topLeft;

	topLeft.m_x = m_center.m_x - ((float)m_width / 2.0f);
	topLeft.m_y = m_center.m_y - ((float)m_height / 2.0f);

	if (m_rotation != 0.0f)
	{
		topLeft =
			rotatePoint(m_center, topLeft, m_rotation);
	}

	return topLeft;
}

//=============================================================================
// Function: const Vector2D getTopRight() const
// Description:
// Gets the coordinates of the top right corner.
// Output:
// const Vector2D
// Returns a vector containing the coordinates of the top right point.
//=============================================================================
const Vector2D Rectangle::getTopRight() const
{
	Vector2D topRight;

	topRight.m_x = m_center.m_x + ((float)m_width / 2.0f);
	topRight.m_y = m_center.m_y - ((float)m_height / 2.0f);
	
	if (m_rotation != 0.0f)
	{
		topRight =
			rotatePoint(m_center, topRight, m_rotation);
	}

	return topRight;
}

//=============================================================================
// Function: const Vector2D getBottomLeft() const
// Description:
// Gets the coordinates of the bottom left corner.
// Output:
// const Vector2D
// Returns a vector containing the coordinates of the bottom left point.
//=============================================================================
const Vector2D Rectangle::getBottomLeft() const
{
	Vector2D bottomLeft;

	bottomLeft.m_x = m_center.m_x - ((float)m_width / 2.0f);
	bottomLeft.m_y = m_center.m_y + ((float)m_height / 2.0f);

	if (m_rotation != 0.0f)
	{
		bottomLeft =
			rotatePoint(m_center, bottomLeft, m_rotation);
	}

	return bottomLeft;
}

//=============================================================================
// Function: const Vector2D getBottomRight() const
// Description:
// Gets the coordinates of the bottom right corner.
// Output:
// const Vector2D
// Returns a vector for the bottom right.
//=============================================================================
const Vector2D Rectangle::getBottomRight() const
{
	Vector2D bottomRight;

	bottomRight.m_x = m_center.m_x + ((float)m_width / 2.0f);
	bottomRight.m_y = m_center.m_y + ((float)m_height / 2.0f);
	
	if (m_rotation != 0.0f)
	{
		bottomRight = 
			rotatePoint(m_center, bottomRight, m_rotation);
	}

	return bottomRight;
}

//=============================================================================
// Function: const int getWidth() const
// Description:
// Gets the width of the rectangle.
// Output:
// const int
// Returns the width of the rectangle.
//=============================================================================
const int Rectangle::getWidth() const
{
	return m_width;
}

//=============================================================================
// Function: const int getHeight() const
// Description:
// Gets the height of the rectangle.
// Output:
// const int
// Returns the height of the rectangle.
//=============================================================================
const int Rectangle::getHeight() const
{
	return m_height;
}

//=============================================================================
// Function: const float getRotation() const
// Description:
// Gets the amount the rectangle is rotated by.
// Output:
// const float
// Returns the rotation of the rectangle.
//=============================================================================
const float Rectangle::getRotation() const
{
	return m_rotation;
}

//=============================================================================
// Function: const float getMinX() const
// Description:
// Gets the smallest x point of the rectangle.
// Output:
// const float
// Returns the smallest x point.
//=============================================================================
const float Rectangle::getMinX() const
{
	float min = m_center.m_x - (float)(m_width / 2);

	if (m_rotation != 0.0f)
	{
		Vector2D topLeft = getTopLeft();
		Vector2D topRight = getTopRight();
		Vector2D bottomLeft = getBottomLeft();
		Vector2D bottomRight = getBottomRight();

		if (topLeft.m_x < min)
		{
			min = topLeft.m_x;
		}

		if (topRight.m_x < min)
		{
			min = topRight.m_x;
		}
		
		if (bottomLeft.m_x < min)
		{
			min = bottomLeft.m_x;
		}

		if (bottomRight.m_x < min)
		{
			min = bottomRight.m_x;
		}
	}

	return min;
}

//=============================================================================
// Function: const float getMinY() const
// Description:
// Gets the smallest y point of the rectangle.
// Output:
// const float
// Returns the smallest y point.
//=============================================================================
const float Rectangle::getMinY() const
{
	float min = m_center.m_y - (float)(m_height / 2);

	if (m_rotation != 0.0f)
	{
		Vector2D topLeft = getTopLeft();
		Vector2D topRight = getTopRight();
		Vector2D bottomLeft = getBottomLeft();
		Vector2D bottomRight = getBottomRight();

		if (topLeft.m_y < min)
		{
			min = topLeft.m_y;
		}

		if (topRight.m_y < min)
		{
			min = topRight.m_y;
		}

		if (bottomLeft.m_y < min)
		{
			min = bottomLeft.m_y;
		}

		if (bottomRight.m_y < min)
		{
			min = bottomRight.m_y;
		}
	}

	return min;
}

//=============================================================================
// Function: const float getMaxX() const
// Description:
// Gets the largest x point.
// Output:
// const float
// Returns the largest x point.
//=============================================================================
const float Rectangle::getMaxX() const
{
	float max = m_center.m_x + (float)(m_width / 2);

	if (m_rotation != 0.0f)
	{
		Vector2D topLeft = getTopLeft();
		Vector2D topRight = getTopRight();
		Vector2D bottomLeft = getBottomLeft();
		Vector2D bottomRight = getBottomRight();

		if (max < topLeft.m_x)
		{
			max = topLeft.m_x;
		}

		if (max < topRight.m_x)
		{
			max = topRight.m_x;
		}

		if (max < bottomLeft.m_x)
		{
			max = bottomLeft.m_x;
		}

		if (max < bottomRight.m_x)
		{
			max = bottomRight.m_x;
		}
	}

	return max;
}

//=============================================================================
// Function: const float getMaxY() const
// Description:
// Gets the largest y point.
// Output:
// const float
// Returns the largest y point.
//=============================================================================
const float Rectangle::getMaxY() const
{
	float max = m_center.m_y + (float)(m_height / 2);

	if (m_rotation != 0.0f)
	{
		Vector2D topLeft = getTopLeft();
		Vector2D topRight = getTopRight();
		Vector2D bottomLeft = getBottomLeft();
		Vector2D bottomRight = getBottomRight();

		if (max < topLeft.m_y)
		{
			max = topLeft.m_y;
		}

		if (max < topRight.m_y)
		{
			max = topRight.m_y;
		}

		if (max < bottomLeft.m_y)
		{
			max = bottomLeft.m_y;
		}

		if (max < bottomRight.m_y)
		{
			max = bottomRight.m_y;
		}
	}

	return max;
}

//=============================================================================
// Function: void setCenter(const float, const float)
// Description:
// Sets the center to the specified coordinates.
// Parameters:
// const float x - The x position.
// const float y - The y position.
//=============================================================================
void Rectangle::setCenter(const float x, const float y)
{
	m_center.m_x = x;
	m_center.m_y = y;
}

//=============================================================================
// Function: void setCenter(const Vector2D&)
// Description:
// Sets the center position of the rectangle.
// Parameters:
// const Vector2D& center - The new center position.
//=============================================================================
void Rectangle::setCenter(const Vector2D& center)
{
	m_center = center;
}

//=============================================================================
// Function: void setWidth(const int)
// Description:
// Sets the width of the rectangle.
// Parameters:
// const int width - The new width.
//=============================================================================
void Rectangle::setWidth(const int width)
{
	if (0 < width)
	{
		m_width = width;
	}
}

//=============================================================================
// Function: void setHeight(const int)
// Description:
// Sets the height of the rectangle.
// Parameters:
// const int height - The new height.
//=============================================================================
void Rectangle::setHeight(const int height)
{
	if (0 < height)
	{
		m_height = height;
	}
}

//=============================================================================
// Function: void setRotation(const float)
// Description:
// Sets the rotation amount of the rectangle.
// Parameters:
// const float rotation - The new rotation. 
//=============================================================================
void Rectangle::setRotation(const float rotation)
{
	m_rotation = rotation;
}

//=============================================================================
// Function: BB_fstream& operator<<(BB_fstream&, const Rectangle&)
// Description: 
// Writes the information of a rectangle out to a BB file stream.
// Parameters: 
// BB_fstream& bbstream - The file stream to read to.
// const Rectangle& rect - The rect to read.
// Output: 
// BB_fstream&
// Returns the modified stream.
//=============================================================================
BB_fstream& operator<<(BB_fstream& bbstream,
	const Rectangle& rect)
{
	if (bbstream.isOpen())
	{
		bbstream << rect.getWidth();
		bbstream << rect.getHeight();
		bbstream << rect.getRotation();
		bbstream << rect.getCenter();
	}

	return bbstream;
}

//=============================================================================
// Function: BB_fstream& operator>>(BB_fstream&, Rectangle&)
// Description: 
// Reads information from the BB file stream into the rect.
// Parameters: 
// BB_fstream& bbstream - The BB file stream to read from.
// Rectangle& rect - The rectangle to read into.
// Output: 
// BB_fstream&
// Returns the modified stream.
//=============================================================================
BB_fstream& operator>>(BB_fstream& bbstream,
	Rectangle& rect)
{
	int width = 0;
	int height = 0;
	float rotation = 0.0f;
	Vector2D center;

	bbstream >> width;
	bbstream >> height;
	bbstream >> rotation;
	bbstream >> center;

	rect.setWidth(width);
	rect.setHeight(height);
	rect.setCenter(center);
	rect.setRotation(rotation);

	return bbstream;
}

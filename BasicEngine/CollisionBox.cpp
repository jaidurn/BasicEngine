#include "CollisionBox.h"



CollisionBox::CollisionBox(const Rectangle& box,
	const bool solid)
	:m_box(box),
	m_solid(solid)
{

}

CollisionBox::CollisionBox(const CollisionBox& box)
	:m_box(box.getBox()),
	m_solid(box.getSolid())
{

}

CollisionBox::~CollisionBox()
{

}

CollisionBox& CollisionBox::operator=(const CollisionBox& box)
{
	if (&box == this)
	{
		return *this;
	}

	m_box = box.m_box;
	m_solid = box.m_solid;
}

//=============================================================================
// Function: const Vector2D& getPosition() const
// Description:
// Gets the center position of the box.
// Output:
// const Vector2D&
// Returns the center position.
//=============================================================================
const Vector2D& CollisionBox::getPosition() const
{
	return m_box.getCenter();
}

//=============================================================================
// Function: const Rectangle& getBox() const
// Description:
// Gets the box containing size information.
// Output:
// const Rectangle&
// Returns a reference to the box object.
//=============================================================================
const Rectangle& CollisionBox::getBox() const
{
	return m_box;
}

//=============================================================================
// Function: const bool getSolid() const
// Description:
// Gets the solid state.
// Output:
// const bool
// Returns true if solid.
// Returns false if not.
//=============================================================================
const bool CollisionBox::getSolid() const
{
	return m_solid;
}

//=============================================================================
// Function: void setPosition(const Vector2D&)
// Description:
// Sets the center position of the box.
// Parameters:
// const Vector2D& position - The new position.
//=============================================================================
void CollisionBox::setPosition(const Vector2D& position)
{
	m_box.setCenter(position);
}

//=============================================================================
// Function: void setBox(const Rectangle& box)
// Description:
// Sets the size information.
// Parameters:
// const Rectangle& box - The new information to use.
//=============================================================================
void CollisionBox::setBox(const Rectangle& box)
{
	m_box = box;
}

//=============================================================================
// Function: void setSolid(const bool)
// Description:
// Sets the solid state of the collision.
// Parameters:
// const bool solid - The solid state to set.
//=============================================================================
void CollisionBox::setSolid(const bool solid)
{
	m_solid = solid;
}
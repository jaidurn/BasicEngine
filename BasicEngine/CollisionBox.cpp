#include "CollisionBox.h"

CollisionBox::CollisionBox()
	:m_box(), m_solid(true)
{

}

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

	return *this;
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

//=============================================================================
// Function: BB_fstream& operator<<(BB_fstream&,
// const CollisionBox&)
// Description: 
// Reads the data from the Collision Box into the BB
// file stream.
// Parameters: 
// BB_fstream& bbstream - The stream to write to.
// const CollisionBox& box - The box to read the data from.
// Output: 
// BB_fstream&
// Returns a reference to the modified BB file stream.
//=============================================================================
BB_fstream& operator<<(BB_fstream& bbstream,
	const CollisionBox& box)
{
	bbstream << box.m_box;
	bbstream << box.m_solid;

	return bbstream;
}

//=============================================================================
// Function: BB_fstream& operator>>(BB_fstream&,
// CollisionBox&)
// Description: 
// Reads the information from the BB file stream and 
// writes it into the box.
// Parameters: 
// BB_fstream& bbstream - The stream to read from.
// CollisionBox& box - The box to write into.
// Output: 
// BB_fstream&
// Returns a reference to the modified BB file stream.
//=============================================================================
BB_fstream& operator>>(BB_fstream& bbstream,
	CollisionBox& box)
{
	bbstream >> box.m_box;
	bbstream >> box.m_solid;

	return bbstream;
}
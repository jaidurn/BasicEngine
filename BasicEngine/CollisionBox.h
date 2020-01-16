#pragma once
//==========================================================================================
// File Name: CollisionBox.h
// Author: Brian Blackmon
// Date Created: 10/31/2019
// Purpose: 
// Handles the information related to collision boxes.
//==========================================================================================
#include "Rectangle.h"
#include "BB_fstream.h"

class CollisionBox
{
public:
	CollisionBox();
	CollisionBox(const Rectangle& box, 
		const bool solid);
	CollisionBox(const CollisionBox& box);
	~CollisionBox();

	CollisionBox& operator= (const CollisionBox& box);

	const Vector2D& getPosition() const;
	const Rectangle& getBox() const;
	const bool getSolid() const;

	void setPosition(const Vector2D& position);
	void setBox(const Rectangle& box);
	void setSolid(const bool solid);

	friend BB_fstream& operator<<(BB_fstream& bbstream,
		const CollisionBox& box);
	
	friend BB_fstream& operator>>(BB_fstream& bbstream,
		CollisionBox& box);

private:
	Rectangle m_box;
	bool m_solid;
};


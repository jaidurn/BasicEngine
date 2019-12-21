#pragma once
//==========================================================================================
// File Name: EntityData.h
// Author: Brian Blackmon
// Date Created: 12/10/2019
// Purpose: 
// Holds base information about an entity.
// This can be used to make copies for multiple entities.
//==========================================================================================
class CollisionBox;
class Sprite;
class Animator;

struct EntityData
{
	int m_instanceCount;
	CollisionBox* m_collisionBox;
	Sprite* m_sprite;
	Animator* m_animator;
};
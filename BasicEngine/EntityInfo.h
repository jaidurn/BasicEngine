#pragma once
//==========================================================================================
// File Name: EntityInfo.h
// Author: Brian Blackmon
// Date Created: 12/10/2019
// Purpose: 
// Holds base information about an entity.
// This can be used to make copies for multiple entities.
//==========================================================================================
#include "CollisionBox.h"
#include "Sprite.h"
#include "Animator.h"
#include "BB_fstream.h"

struct EntityInfo
{
	EntityInfo()
		:m_instanceCount(1),
		m_persistant(false),
		m_collisionBox(nullptr),
		m_sprite(nullptr),
		m_animator(nullptr)
	{
	}

	~EntityInfo()
	{
		if (m_collisionBox)
		{
			delete m_collisionBox;
		}

		if (m_sprite)
		{
			delete m_sprite;
		}

		if (m_animator)
		{
			delete m_animator;
		}
	}

	int m_instanceCount;
	bool m_persistant;
	CollisionBox* m_collisionBox;
	Sprite* m_sprite;
	Animator* m_animator;
};
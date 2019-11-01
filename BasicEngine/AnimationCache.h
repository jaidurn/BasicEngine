#pragma once
//==========================================================================================
// File Name: AnimationCache.h
// Author: Brian Blackmon
// Date Created: 10/29/2019
// Purpose: 
// Holds each unique instance of an animation, allowing for less data
// to be required for an animator.
//==========================================================================================
#include <map>
#include <string>

typedef std::string string;

class AnimationSet;

class AnimationCache
{
public:
	AnimationCache();
	~AnimationCache();

	const AnimationSet* getAnimationSet(const string name) const;

	const bool animationSetExists(const string name) const;
	
	void addAnimationSet(const string name, AnimationSet* set);

private:
	std::map<string, AnimationSet*>	m_cache;

	void cleanUp();
};


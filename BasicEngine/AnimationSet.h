#pragma once
//==========================================================================================
// File Name: AnimationSet.h
// Author: Brian Blackmon
// Date Created: 10/23/2019
// Purpose: 
// Holds a collection of animations for a texture.
//==========================================================================================
#include "Animation.h"

class AnimationSet
{
public:
	AnimationSet(const string setPath);
	~AnimationSet();

	Animation* getAnimation(const string name) const;
	const Rectangle* getFrame(const string name,
							  const int index) const;
	const int getInstanceCount() const;
	const int getAnimationCount() const;
	const string getPath() const;

	void addAnimation(Animation *animation);

	const bool animationExists(const string name) const;

	void addInstance();
	void removeInstance();

	friend BB_fstream& operator<<(BB_fstream& bbstream,
		const AnimationSet& set);

	friend BB_fstream& operator>>(BB_fstream& bbstream,
		AnimationSet& set);

private:
	std::vector<Animation*> m_animations;
	int m_instanceCount;
	string m_path;
};
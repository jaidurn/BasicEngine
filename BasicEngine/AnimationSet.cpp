#include "AnimationSet.h"

AnimationSet::AnimationSet()
	:m_instanceCount(0)
{

}

AnimationSet::~AnimationSet()
{
	for (unsigned int i = 0; i < m_animations.size(); i++)
	{
		delete m_animations[i];
		m_animations[i] = NULL;
	}

	m_animations.resize(0);
}

//=============================================================================
// Function: Animation* getAnimation(const string) const
// Description:
// Gets the animation with the specified name.
// Parameters:
// const string name - The name of the animation to get.
// Output:
// Animation*
// On success - Returns the found animation.
// On failure - Returns NULL.
//=============================================================================
Animation* AnimationSet::getAnimation(const string name) const
{
	Animation *animation = NULL;

	for (unsigned int i = 0; i < m_animations.size(); i++)
	{
		if (m_animations[i]->getName() == name)
		{
			animation = m_animations[i];
			break;
		}
	}

	return animation;
}

//=============================================================================
// Function: const Rectangle* getFrame(const string, const int) const
// Description:
// Gets the frame at the specified index of the animation.
// Parameters:
// const string name - The name of the animation to get the frame
// from. 
// const int index - The index of the frame to get.
// Output:
// const Rectangle*
// Returns the rectangle with the current frame.
// If there is no current frame, it returns NULL
//=============================================================================
const Rectangle* AnimationSet::getFrame(const string name,
	const int index) const
{
	Animation *animation = getAnimation(name);

	if (animation)
	{
		return &animation->operator[](index);
	}
	
	return NULL;
}

//=============================================================================
// Function: const int getInstanceCount() const
// Description:
// Gets the current number of instances that are pointing to
// the animation set.
// Output:
// const int
// Returns the current number of instances pointing to the animation
// set.
//=============================================================================
const int AnimationSet::getInstanceCount() const
{
	return m_instanceCount;
}

//=============================================================================
// Function: void addAnimation(Animation *animation)
// Description:
// Adds an animation to the animation set.
// Parameters:
// Animation *animation - The animation to add.
//=============================================================================
void AnimationSet::addAnimation(Animation *animation)
{
	if (animation)
	{
		m_animations.push_back(animation);
	}
}

//=============================================================================
// Function: const bool animationExists(const string)
// Description:
// Checks to see if an animation with the specified name exists.
// Parameters:
// const string name - The name of the animation to find.
// Output:
// const bool
// Returns true if the animation exists.
// Returns false if the animation doesn't exist.
//=============================================================================
const bool AnimationSet::animationExists(const string name) const
{
	bool exists = false;

	Animation *animation = getAnimation(name);

	if (animation)
	{
		exists = true;
	}

	return exists;
}

//=============================================================================
// Function: void addInstance()
// Description:
// Increases the instance count of the animation set by one.
//=============================================================================
void AnimationSet::addInstance()
{
	m_instanceCount++;
}

//=============================================================================
// Function: void removeInstance()
// Description:
// Decreases the instance count of the animation by one.
//=============================================================================
void AnimationSet::removeInstance()
{
	m_instanceCount--;
}
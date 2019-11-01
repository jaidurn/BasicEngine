#include "Animator.h"
#include "AnimationSet.h"
#include "Animation.h"

Animator::Animator(const AnimationSet* animationSet)
	:
	m_animationSet(animationSet),
	m_animation(""),
	m_speed(0.0f),
	m_currentFrame(0.0f),
	m_loop(false),
	m_active(true)
{

}

Animator::Animator(const Animator& animator)
	:m_animationSet(NULL),
	m_animation(""),
	m_currentFrame(0.0f),
	m_speed(0.0f),
	m_loop(false),
	m_active(true)
{
	m_animationSet = animator.m_animationSet;
	m_animation = animator.m_animation;
	m_currentFrame = animator.m_currentFrame;
	m_speed = animator.m_speed;
	m_loop = animator.m_loop;
	m_active = animator.m_active;
}

Animator::~Animator()
{
	m_animationSet = NULL;
}

//=============================================================================
// Function: Animator& operator++()
// Description:
// A pre-increment operator for the Animator.
// Output:
// Animator&
// Returns a reference to the incremented animator.
//=============================================================================
Animator& Animator::operator++()
{
	m_currentFrame += m_speed;

	if (m_animationSet)
	{
		Animation *animation = m_animationSet->getAnimation(m_animation);

		if (animation)
		{
			if (animation->getFrameCount() < m_currentFrame)
			{
				if (!m_loop)
				{
					m_currentFrame = (float)animation->getFrameCount();
				}
				else
				{
					m_currentFrame = 0.0f;
				}
			}
			else if (m_currentFrame < 0.0f)
			{
				if (!m_loop)
				{
					m_currentFrame = 0.0f;
				}
				else
				{
					m_currentFrame = (float)animation->getFrameCount();
				}
			}
		}
	}

	return *this;
}

//=============================================================================
// Function: Animator operator++(int)
// Description:
// A post-increment operator for the Animator.
// Output:
// Animator&
// Returns the object state before incremementing.
//=============================================================================
Animator Animator::operator++(int)
{
	Animator temp = *this;

	m_currentFrame += m_speed;

	if (m_animationSet)
	{
		Animation *animation = m_animationSet->getAnimation(m_animation);

		if (animation)
		{
			if (animation->getFrameCount() < m_currentFrame)
			{
				if (!m_loop)
				{
					m_currentFrame = (float)animation->getFrameCount();
				}
				else
				{
					m_currentFrame = 0.0f;
				}
			}
			else if (m_currentFrame < 0.0f)
			{
				if (!m_loop)
				{
					m_currentFrame = 0.0f;
				}
				else
				{
					m_currentFrame = (float)animation->getFrameCount();
				}
			}
		}
	}

	return temp;
}

//=============================================================================
// Function: const string getAnimation() const
// Description:
// Gets the name of the current animation being held.
// Output:
// const string
// Returns the name of the animation.
// Returns "" if there isn't one.
//=============================================================================
const string Animator::getAnimation() const
{
	return m_animation;
}

//=============================================================================
// Function: const int getCurrentFrame() const
// Description:
// Gets the current frame index of the component.
// Output:
// const int 
// Returns the current frame as an int.
//=============================================================================
const int Animator::getCurrentFrame() const
{
	return (int)m_currentFrame;
}

//=============================================================================
// Function: const Rectangle* getFrame() const
// Description:
// Gets the current frame information for the animation
// Output:
// const Rectangle*
// Returns a rectangle containing the frame information.
// If there is no frame information, it returns NULL.
//=============================================================================
const Rectangle* Animator::getFrame() const
{
	if (m_animationSet)
	{
		return m_animationSet->getFrame(m_animation, (int)m_currentFrame);
	}

	return NULL;
}

//=============================================================================
// Function: const float getSpeed() const
// Description:
// Gets the speed of the component.
// Output:
// const float
// Returns the speed 
//=============================================================================
const float Animator::getSpeed() const
{
	return m_speed;
}

//=============================================================================
// Function: const bool getLoop() const
// Description:
// Checks if the component can loop the animation or not.
// Output:
// const bool
// Returns true if the animation loops.
// Returns false if the animation runs once.
//=============================================================================
const bool Animator::getLoop() const
{
	return m_loop;
}

//=============================================================================
// Function: const bool getActive() const
// Description:
// Gets the active state of the animator.
// Output:
// const bool
// Returns true if the animator is active.
// Returns false if inactive.
//=============================================================================
const bool Animator::getActive() const
{
	return m_active;
}

//=============================================================================
// Function: void setAnimationSet(const AnimationSet*)
// Description:
// Sets the animation set to use for information.
// Parameters:
// const AnimationSet* animationSet - The animation set to use for 
// frame information.
//=============================================================================
void Animator::setAnimationSet(const AnimationSet* animationSet)
{
	if (animationSet)
	{
		m_animationSet = animationSet;
	}
}

//=============================================================================
// Function: void setAnimation(const string)
// Description:
// Sets the animation to the specified name.
// Parameters:
// const string name - The name of the animation.
//=============================================================================
void Animator::setAnimation(const string name)
{
	m_animation = name;
	m_currentFrame = 0.0f;
}

//=============================================================================
// Function: void setFrame(const float)
// Description:
// Sets the current frame of the animation to the specified frame.
// Parameters:
// const float frame - The new frame.
//=============================================================================
void Animator::setFrame(const float frame)
{
	if (frame < 0.0f)
	{
		m_currentFrame = 0.0f;
	}
	else
	{
		m_currentFrame = frame;
	}
}

//=============================================================================
// Function: void setSpeed(const float)
// Description:
// Sets the speed of the animation to the specified speed.
// Parameters:
// const float speed - The new speed.
//=============================================================================
void Animator::setSpeed(const float speed)
{
	m_speed = speed;
}

//=============================================================================
// Function: void setLoop(const bool)
// Description:
// Sets if the animation loops or not.
// Parameters:
// const bool loop - The new loop state.
//=============================================================================
void Animator::setLoop(const bool loop)
{
	m_loop = loop;
}

//=============================================================================
// Function: void setActive(const bool)
// Description:
// Sets the active state of the animator.
// Parameters:
// const bool active - The new active state to set.
//=============================================================================
void Animator::setActive(const bool active)
{
	m_active = active;
}
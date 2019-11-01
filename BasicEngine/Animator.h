#pragma once
//==========================================================================================
// File Name: Animator.h
// Author: Brian Blackmon
// Date Created: 10/24/2019
// Purpose: 
// Holds instance specific information for animations.
//==========================================================================================
#include <string>
#include "Rectangle.h"

typedef std::string string;

class AnimationSet;

class Animator
{
public:
	Animator(const AnimationSet* animationSet);
	Animator(const Animator& animator);
	~Animator();
	
	Animator& operator++(); // Pre
	Animator operator++(int); // Post

	const string getAnimation() const;
	const int getCurrentFrame() const;
	const Rectangle* getFrame() const;
	const float getSpeed() const;
	const bool getLoop() const;
	const bool getActive() const;

	void setAnimationSet(const AnimationSet* animationSet);
	void setAnimation(const string name);
	void setFrame(const float frame);
	void setSpeed(const float speed);
	void setLoop(const bool loop);
	void setActive(const bool active);

private:
	const AnimationSet* m_animationSet;

	string m_animation;
	float m_currentFrame;
	float m_speed;
	bool m_loop;
	bool m_active;
};


#pragma once
//==========================================================================================
// File Name: Animation.h
// Author: Brian Blackmon
// Date Created: 10/23/2019
// Purpose: 
// Holds information on an animation.
//==========================================================================================
#include "Rectangle.h"
#include <vector>
#include <string>

typedef std::string string;

class Animation
{
public:
	Animation(const string name,
		const float speed);
	~Animation();

	const Rectangle& operator[](const int index) const;

	const string getName() const;
	const float getSpeed() const;
	const int getFrameCount() const;

	void addFrame(const Rectangle& frame);

private:
	string m_name;
	float m_speed;

	std::vector<Rectangle> m_frames;
};


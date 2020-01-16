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

#include "BB_fstream.h"

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
	void setSpeed(const float speed);
	void setName(const string name);

private:
	string m_name;
	float m_speed;

	std::vector<Rectangle> m_frames;
};

BB_fstream& operator<<(BB_fstream& bbstream, const Animation& animation);
BB_fstream& operator>>(BB_fstream& bbstream, Animation& animation);
#include "Animation.h"

Animation::Animation(const string name,
	const float speed)
	:m_name(name),
	m_speed(speed)
{

}

Animation::~Animation()
{
	m_frames.clear();
	m_frames.resize(0);
}

//=============================================================================
// Function: const Rectangle& operator[](const int) const
// Description:
// Overloads the [] operator, allowing for faster indexing.
// It includes error checking!!
// Parameters:
// const int index - The index of the rectangle to get.
// Output:
// const Rectangle&
// Returns the frame found.
//=============================================================================
const Rectangle& Animation::operator[](const int index) const
{
	int workingIndex = index;

	if (m_frames.size() == 0)
	{
		return Rectangle(Vector2D(0.0f, 0.0f), 0, 0);
	}

	if (workingIndex < 0)
	{
		workingIndex = 0;
	}
	else if ((int)m_frames.size() <= workingIndex)
	{
		workingIndex = (int)m_frames.size() - 1;
	}

	return m_frames[workingIndex];
}

//=============================================================================
// Function: const string getName() const
// Description:
// Gets the animation name.
// Output:
// const string
// Returns the name of the animation.
//=============================================================================
const string Animation::getName() const
{
	return m_name;
}

//=============================================================================
// Function: const float getSpeed() const
// Description:
// Gets the default speed of the animation.
// Output:
// const float
// Returns the speed.
//=============================================================================
const float Animation::getSpeed() const
{
	return m_speed;
}

//=============================================================================
// Function: const int getFrameCount() const
// Description:
// Gets the number of frames the animation currently has.
// Output:
// const int
// Returns the frame count of the animation.
//=============================================================================
const int Animation::getFrameCount() const
{
	return (int)m_frames.size();
}

//=============================================================================
// Function: void addFrame(const Rectangle& frame)
// Description:
// Adds a frame to the current animation.
// Parameters:
// const Rectangle& frame - The frame to add.
//=============================================================================
void Animation::addFrame(const Rectangle& frame)
{
	m_frames.push_back(frame);
}

//=============================================================================
// Function: void setSpeed(const float)
// Description: 
// Sets the base speed for the animation.
// Parameters: 
// const float speed - The new speed to use for the animation.
//=============================================================================
void Animation::setSpeed(const float speed)
{
	m_speed = speed;
}

//=============================================================================
// Function: void setName(const string)
// Description: 
// Sets the name of the animation.
// Parameters: 
// const string name - The name to use for the animiation.
//=============================================================================
void Animation::setName(const string name)
{
	if (name != "")
	{
		m_name = name;
	}
}

//=============================================================================
// Function: BB_fstream& operator<<(BB_fstream&, const Animation&)
// Description: 
// Reads animation information out to a bb stream.
// Parameters: 
// BB_fstream& bbstream - The stream to read.
// const Animation& animation - The animation to read out.
// Output: 
// BB_fstream&
// Returns the modified BB_fstream.
//=============================================================================
BB_fstream& operator<<(BB_fstream& bbstream, 
	const Animation& animation)
{
	if (bbstream.isOpen())
	{
		
		bbstream << animation.getFrameCount();
		bbstream << animation.getSpeed();

		for (int i = 0; i < animation.getFrameCount(); i++)
		{
			Rectangle rect = animation[i];

			bbstream << rect;
		}

		bbstream << animation.getName();
	}

	return bbstream;
}

//=============================================================================
// Function: BB_fstream& operator>>(BB_fstream&, Animation&) 
// Description: 
// Reads information from the BB stream into the animation 
// object.
// Parameters: 
// BB_fstream& bbstream - The BB file stream to read from.
// Animation& animation - The animation to read into.
// Output: 
// BB_fstream&
// Returns the modified file stream.
//=============================================================================
BB_fstream& operator>>(BB_fstream& bbstream,
	Animation& animation)
{
	int frameCount = 0;
	float speed = 1.0f;

	bbstream >> frameCount;
	bbstream >> speed;

	animation.setSpeed(speed);

	for (int i = 0; i < frameCount; i++)
	{
		Rectangle rect;

		bbstream >> rect;

		animation.addFrame(rect);
	}

	string name = "";

	bbstream >> name;

	animation.setName(name);

	return bbstream;
}
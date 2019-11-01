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
#pragma once
//==========================================================================================
// File Name: Camera2D.h 
// Author: Brian Blackmon
// Date Created: 10/24/2019
// Purpose: 
// Holds information for 2D cameras.
//==========================================================================================
#include "Rectangle.h"
#include <vector>

class Camera2D
{
public:
	Camera2D(const Rectangle& size,
		const Rectangle& bounds);
	~Camera2D();

	const Vector2D& getPosition() const;
	const Rectangle& getSize() const;
	const Rectangle& getBounds() const;

	const float getCurrentScaleX() const;
	const float getCurrentScaleY() const;

	void setMinScaleX(const float xMinScale);
	void setMinScaleY(const float yMinScale);

	void setMaxScaleX(const float xMaxScale);
	void setMaxScaleY(const float yMaxScale);

	void setCurrentScaleX(const float xCurrentScale);
	void setCurrentScaleY(const float yCurrentScale);

	void setPosition(const Vector2D& position);
	void setSize(const Rectangle& size);
	void setBounds(const Rectangle& bounds);

	void addTargetID(const int targetID);
	void removeTargetID(const int targetID);

private:
	std::vector<int> m_targets;

	Rectangle m_size;
	Rectangle m_bounds;

	float m_xMinScale;
	float m_yMinScale;

	float m_xMaxScale;
	float m_yMaxScale;

	float m_xBaseScale;
	float m_yBaseScale;

	float m_xCurrentScale;
	float m_yCurrentScale;
};


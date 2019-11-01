#include "Camera2D.h"

Camera2D::Camera2D(const Rectangle& size,
	const Rectangle& bounds)
	:m_size(size),
	m_bounds(bounds),
	m_xBaseScale(1.0f),
	m_yBaseScale(1.0f),
	m_xMinScale(0.5f),
	m_yMinScale(0.5f),
	m_xMaxScale(5.0f),
	m_yMaxScale(5.0f),
	m_xCurrentScale(m_xBaseScale),
	m_yCurrentScale(m_yBaseScale)
{

}

Camera2D::~Camera2D()
{

}

//=============================================================================
// Function: const Vector2D& getPosition() const
// Description:
// Gets the center position of the camera.
// Output:
// const Vector2D&
// Returns the center position of the camera.
//=============================================================================
const Vector2D& Camera2D::getPosition() const
{
	return m_size.getCenter();
}

//=============================================================================
// Function: const Rectangle& getSize() const
// Description:
// Gets the current size of the camera.
// Output:
// const Rectangle&
// Returns the size.
//=============================================================================
const Rectangle& Camera2D::getSize() const
{
	return m_size;
}

//=============================================================================
// Function: const Rectangle& getBounds() const
// Description:
// Gets the bounds of the camera. If the target is outside of them, 
// then they should move.
// Output:
// const Rectangle&
// Returns the bound rectangle.
//=============================================================================
const Rectangle& Camera2D::getBounds() const
{
	return m_bounds;
}

//=============================================================================
// Function: const float getCurrentScaleX() const
// Description:
// Gets the current X scale.
// Output:
// const float
// Returns the current scale.
//=============================================================================
const float Camera2D::getCurrentScaleX() const
{
	return m_xCurrentScale;
}

//=============================================================================
// Function: const float getCurrentScaleY() const
// Description:
// Gets the current Y scale.
// Output:
// const float
// Returns the current scale.
//=============================================================================
const float Camera2D::getCurrentScaleY() const
{
	return m_yCurrentScale;
}

//=============================================================================
// Function: void setMinScaleX(const float)
// Description:
// Sets the minimum scale that the camera can have for its x.
// Parameters:
// const float xMinScale - The new X minimum scale.
//=============================================================================
void Camera2D::setMinScaleX(const float xMinScale)
{
	if (0.1f < xMinScale && xMinScale < m_xMaxScale)
	{
		m_xMinScale = xMinScale;
	}
}

//=============================================================================
// Function: void setMinScaleY(const float)
// Description:
// Sets the minimum scale that the camera can have for its y.
// Parameters:
// const float yMinScale - The new y minimum scale.
//=============================================================================
void Camera2D::setMinScaleY(const float yMinScale)
{
	if (0.1f < yMinScale && yMinScale < m_yMaxScale)
	{
		m_yMinScale = yMinScale;
	}
}

//=============================================================================
// Function: void setMaxScaleX(const float)
// Description:
// Sets the maximum x scale the camera can have.
// Parameters:
// const float xMaxScale - The new max x.
//=============================================================================
void Camera2D::setMaxScaleX(const float xMaxScale)
{
	if (m_xMinScale < xMaxScale)
	{
		m_xMaxScale = xMaxScale;
	}
}

//=============================================================================
// Function: void setMaxScaleY(const float)
// Description:
// Sets the maximum y scale that the camera can have.
// Parameters:
// const float yMaxScale - The new max y.
//=============================================================================
void Camera2D::setMaxScaleY(const float yMaxScale)
{
	if (m_yMinScale < yMaxScale)
	{
		m_yMaxScale = yMaxScale;
	}
}

//=============================================================================
// Function: void setCurrentScaleX(const float)
// Description:
// Sets the current x scale.
// Parameters:
// const float xCurrentScale - The new current scale.
//=============================================================================
void Camera2D::setCurrentScaleX(const float xCurrentScale)
{
	if (xCurrentScale < m_xMinScale)
	{
		m_xCurrentScale = m_xMinScale;
	}
	else if (m_xMaxScale < xCurrentScale)
	{
		m_xCurrentScale = m_xMaxScale;
	}
	else
	{
		m_xCurrentScale = xCurrentScale;
	}
}

//=============================================================================
// Function: void setCurrentScaleY(const float)
// Description:
// Sets the current y scale.
// Parameters:
// const float yCurrentScale - The new current y scale.
//=============================================================================
void Camera2D::setCurrentScaleY(const float yCurrentScale)
{
	if (yCurrentScale < m_yMinScale)
	{
		m_yCurrentScale = m_yMinScale;
	}
	else if (m_yMaxScale < yCurrentScale)
	{
		m_yCurrentScale = m_yMaxScale;
	}
	else
	{
		m_yCurrentScale = yCurrentScale;
	}
}

//=============================================================================
// Function: void setPosition(const Vector2D&)
// Description:
// Sets the center position of the camera.
// Parameters:
// const Vector2D& position - The new center position.
//=============================================================================
void Camera2D::setPosition(const Vector2D& position)
{
	m_size.setCenter(position.m_x, position.m_y);
	m_bounds.setCenter(position.m_x, position.m_y);
}

//=============================================================================
// Function: void setSize(const Rectangle&)
// Description:
// Sets the size of the camera.
// Parameters:
// const Rectangle& size - The new size of the camera.
//=============================================================================
void Camera2D::setSize(const Rectangle& size)
{
	m_size = size; 
}

//=============================================================================
// Function: void setBounds(const Rectangle&)
// Description:
// Sets the bounding rect for the camera.
// Parameters:
// const Rectangle& bounds - The new bounds.
//=============================================================================
void Camera2D::setBounds(const Rectangle& bounds)
{
	m_bounds = bounds;
}

//=============================================================================
// Function: void addTargetID(const int)
// Description:
// Adds a target ID to the list of targets. These ids are used
// to determine where the camera is supposed to move to.
// Parameters:
// const int targetID - The id of the target.
//=============================================================================
void Camera2D::addTargetID(const int targetID)
{
	bool exists = false;

	for (unsigned int i = 0; i < m_targets.size(); i++)
	{
		if (m_targets[i] == targetID)
		{
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		m_targets.push_back(targetID);
	}
}

//=============================================================================
// Function: void removeTargetID(const int)
// Description:
// Removes the target ID from the list.
// Parameters:
// const int targetID - The target ID to remove from the list.
//=============================================================================
void Camera2D::removeTargetID(const int targetID)
{
	for (unsigned int i = 0; i < m_targets.size(); i++)
	{
		if (m_targets[i] == targetID)
		{
			auto vit = m_targets.begin();

			std::advance(vit, i);

			vit = m_targets.erase(vit);

			break;
		}
	}
}


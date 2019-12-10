#pragma once
//==========================================================================================
// File Name: KBM.h
// Author: Brian Blackmon
// Date Created: 12/3/2019
// Purpose: 
// Wraps the keyboard and mouse input devices into one.
//==========================================================================================
#include "InputDevice.h"
#include "Vector2D.h"

class KBM :	public InputDevice
{
public:
	KBM(const int deviceID);
	virtual ~KBM();

	virtual const DeviceType getDeviceType() const;
	const Vector2D& getPosition() const;

	void setPosition(const Vector2D& position);

private:
	Vector2D m_position;
};


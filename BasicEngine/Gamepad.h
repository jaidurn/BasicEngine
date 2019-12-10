#pragma once
//==========================================================================================
// File Name: 
// Author: 
// Date Created: 
// Purpose: 
// 
//==========================================================================================
#include "InputDevice.h"
#include "Vector2D.h"

class Gamepad : public InputDevice
{
public:
	Gamepad(const int joystickID);
	virtual ~Gamepad();

	virtual const DeviceType getDeviceType() const;

	const Vector2D& getLeftStick() const;
	const Vector2D& getRightStick() const;
	const float getLeftTrigger() const;
	const float getRightTrigger() const;

	void setLeftX(const Sint16 leftX);
	void setLeftY(const Sint16 leftY);
	
	void setRightX(const Sint16 rightX);
	void setRightY(const Sint16 rightY);

	void setLeftTrigger(const Sint16 leftTrigger);
	void setRightTrigger(const Sint16 rightTrigger);

private:
	const Sint16 m_MAX_AMOUNT = -32768;

	SDL_GameController *m_controller;

	Vector2D m_leftStick;
	Vector2D m_rightStick;

	float m_leftTrigger;
	float m_rightTrigger;

	bool open(const int joystickID);
	void close();
};


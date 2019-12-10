#pragma once
//==========================================================================================
// File Name: InputDevice.h
// Author: Brian Blackmon
// Date Created: 12/3/2019
// Purpose: 
// A base class for input devices. Includes basic things
// such as an abstraction for the SDL input button codes.
//==========================================================================================
#include <SDL.h>
#include <map>
#include <unordered_map>

enum ButtonCode
{
	BTN_ERROR = -1,
	BTN_ANALOG_UP = 0,
	BTN_ANALOG_RIGHT,
	BTN_ANALOG_DOWN,
	BTN_ANALOG_LEFT,
	BTN_SELECT,
	BTN_BASE_ATTACK_0,
	BTN_BASE_ATTACK_1,
	BTN_SPECIAL_ATTACK_0,
	BTN_SPECIAL_ATTACK_1,
	BTN_USE_ITEM,
	BTN_PAUSE,
	BTN_SELECT_ALT
};

enum AxisCode
{
	AXIS_LEFT,
	AXIS_RIGHT,
	AXIS_LEFT_TRIGGER,
	AXIS_RIGHT_TRIGGER
};

class InputDevice
{
public:
	enum DeviceType
	{
		INPUTDEVICE_DEFAULT,
		INPUTDEVICE_KBM,
		INPUTDEVICE_GAMEPAD
	};

	InputDevice(const int deviceID);
	virtual ~InputDevice();

	void pressButton(const Uint32 button);
	void releaseButton(const Uint32 button);
	bool buttonPressed(const ButtonCode buttonCode) const;

	void mapButton(const Uint32 button,
		const ButtonCode buttonCode);
	bool unmapButton(const Uint32 button);

	const ButtonCode getButtonCode(const Uint32 button) const;
	const int getDeviceID() const;
	virtual const DeviceType getDeviceType() const = 0;

protected:
	int m_deviceID;

	std::map<Uint32, ButtonCode> m_buttonMap;
	std::unordered_map<ButtonCode, bool> m_buttonStates;
};


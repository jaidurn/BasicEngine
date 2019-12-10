#pragma once
//==========================================================================================
// File Name: InputMessage.h
// Author: Brian Blackmon
// Date Created: 12/5/2019
// Purpose: 
// A message class for input messages.
// Includes a bunch of different types of input messages.
//==========================================================================================
#include "IMessage.h"
#include "InputDevice.h"
#include "Vector2D.h"

class InputMessage : public IMessage
{
public:
	enum InputType
	{
		INPUT_BUTTON,
		INPUT_MOVE,
		INPUT_AXIS
	};

	InputMessage(const int deviceID)
		:m_deviceID(deviceID)
	{

	}

	virtual ~InputMessage()
	{

	}

	virtual const MessageType getType() const
	{
		return MSG_INPUT;
	}

	virtual const InputType getInputType() const = 0;

	int m_deviceID;
};

class InputButtonMsg : public InputMessage
{
public:
	InputButtonMsg(const int deviceID,
		ButtonCode button,
		bool pressed)
		:InputMessage(deviceID),
		m_button(button),
		m_pressed(pressed)
	{

	}

	virtual ~InputButtonMsg()
	{

	}

	virtual const InputType getInputType() const
	{
		return INPUT_BUTTON;
	}

	ButtonCode m_button;
	bool m_pressed;
};

class InputAxisMsg : public InputMessage
{
public:
	enum AXIS_ID
	{
		AXIS_LEFT,
		AXIS_RIGHT,
		AXIS_LEFT_TRIGGER,
		AXIS_RIGHT_TRIGGER
	};

	InputAxisMsg(const int deviceID,
		AXIS_ID axis,
		const Vector2D& movement)
		:InputMessage(deviceID),
		m_axis(axis),
		m_movement(movement)
	{

	}

	virtual const InputType getInputType() const
	{
		return InputType::INPUT_AXIS;
	}

	AXIS_ID m_axis;
	Vector2D m_movement;
};

class InputMoveMsg : public InputMessage
{
public:
	InputMoveMsg(const int deviceID,
		const Vector2D& position)
		:InputMessage(deviceID),
		m_position(position)
	{

	}

	virtual ~InputMoveMsg() {}

	virtual const InputType getInputType() const
	{
		return InputType::INPUT_MOVE;
	}

	Vector2D m_position;
};

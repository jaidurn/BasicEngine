#pragma once
//==========================================================================================
// File Name: Messages.h
// Author: Brian Blackmon
// Date Created: 12/10/2019
// Purpose: 
// A header containing all of the message structs.
//==========================================================================================
#include "InputDevice.h"
#include "Vector2D.h"

typedef unsigned int Uint32;
const int m_STRING_SIZE = 256;

enum MessageType
{
	MSG_ERROR = -1,
	MSG_INPUT_BUTTON,
	MSG_INPUT_MOUSE,
	MSG_INPUT_AXIS,
	MSG_SYS_QUIT
};

// Error Msg
struct ErrorMessage
{
	char msg[m_STRING_SIZE];
};

// Input Messages
struct InputButtonMsg
{
	int deviceID;
	ButtonCode button;
	bool pressed;
};

struct InputAxisMsg
{
	int deviceID;
	AxisCode axis;
	Vector2D amount;
};

struct InputMouseMsg
{
	int deviceID;
	Vector2D position;
};

// System messages
struct SystemQuit
{
	char quitMsg[m_STRING_SIZE];
};

struct Message
{
	MessageType type;
	Uint32 time;

	union
	{
		ErrorMessage error;
		// TODO: Decide if I want to hide these in an input msg.
		InputButtonMsg button;
		InputAxisMsg axis;
		InputMouseMsg mouse;

		SystemQuit quit;
	};
};
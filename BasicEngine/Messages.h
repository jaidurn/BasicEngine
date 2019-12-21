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
#include <SDL.h>

const int m_STRING_SIZE = 256;

enum MessageType
{
	MSG_ERROR = -1,
	MSG_INPUT_BUTTON,
	MSG_INPUT_MOUSE,
	MSG_INPUT_AXIS,
	MSG_SYS_QUIT,
	MSG_ENTITY_MOVE,
	MSG_ENTITY_CREATE,
	MSG_ENTITY_DESTROY
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
struct SystemQuitMsg
{
	char quitMsg[m_STRING_SIZE];
};

// Entity Messages
struct EntityMoveMsg
{
	Vector2D oldPos;
	Vector2D newPos;
};

struct EntityCreateMsg
{
	int entityType;
	Vector2D position;
};

struct EntityDestroyMsg
{
	Vector2D position;
};

struct EntityMsg
{
	int entityID;

	union
	{
		EntityCreateMsg create;
		EntityDestroyMsg destroy;
		EntityMoveMsg move;
	};
};

struct Message
{
	Message(MessageType msgType)
		:type(msgType),
		time(SDL_GetTicks())
	{

	}

	MessageType type;
	Uint32 time;

	union
	{
		ErrorMessage error;
		// TODO: Decide if I want to hide these in an input msg.
		InputButtonMsg button;
		InputAxisMsg axis;
		InputMouseMsg mouse;

		SystemQuitMsg quit;

		// Entity Messages
		// TODO: Decide if I want to hide these in an entity msg.
		EntityMoveMsg entityMove;
		EntityCreateMsg entityCreate;
		EntityDestroyMsg entityDestroy;
	};
};
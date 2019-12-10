#pragma once
//==========================================================================================
// File Name: IMessage.h
// Author: Brian Blackmon
// Date Created: 12/5/2019
// Purpose: 
// An interface class for messages.
//==========================================================================================
enum MessageType
{
	MSG_ERROR = -1,
	MSG_INPUT,
	MSG_COLLISION,
	MSG_MOVEMENT,
	MSG_QUIT,
	MSG_PAUSE
};

class IMessage
{
public:

	IMessage()
	{

	}

	virtual ~IMessage()
	{

	}

	virtual const MessageType getType() const = 0;
};
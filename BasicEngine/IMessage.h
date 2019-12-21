#pragma once
//==========================================================================================
// File Name: IMessage.h
// Author: Brian Blackmon
// Date Created: 12/5/2019
// Purpose: 
// An interface class for messages.
//==========================================================================================
enum MessageTypeOld
{
	OMSG_ERROR = -1,
	OMSG_INPUT,
	OMSG_COLLISION,
	OMSG_MOVEMENT,
	OMSG_QUIT,
	OMSG_PAUSE
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
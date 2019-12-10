#pragma once
//==========================================================================================
// File Name: MessageLocator.h
// Author: Brian Blackmon
// Date Created: 12/10/2019
// Purpose: 
// A locator instance for getting the message system.
//==========================================================================================
#include "MessageSystem.h"

class MessageLocator
{
public:
	MessageLocator() {}
	~MessageLocator() {}

	static MessageSystem& getMsgSys();
	static void setMsgSys(MessageSystem *msgSys);

private:
	static MessageSystem *m_system;
};

MessageSystem* MessageLocator::m_system = nullptr;

//=============================================================================
// Function: MessageSystem& getMsgSys()
// Description:
// Gets the current message system being held by the locator.
// Output:
// MessageSystem&
// If there isn't a current message system, it makes one with
// the default parameters.
//=============================================================================
MessageSystem& MessageLocator::getMsgSys()
{
	if (m_system == nullptr)
	{
		m_system = new MessageSystem();
	}

	return *m_system;
}

//=============================================================================
// Function: void setMsgSys(MessageSystem*)
// Description:
// Sets the new active message system to use. Deletes the old
// one.
// Parameters:
// MessageSystem *msgSys - The new message system to use.
//=============================================================================
void MessageLocator::setMsgSys(MessageSystem* msgSys)
{
	if (m_system)
	{
		delete m_system;
	}

	m_system = msgSys;
}
#pragma once
//==========================================================================================
// File Name: MessageSystem.h
// Author: Brian Blackmon
// Date Created: 12/9/2019
// Purpose: 
// Handles reading and processing messages.
//==========================================================================================
#include "IMessage.h"
#include <deque>

class MessageSystem
{
public:
	MessageSystem(const int MAX_MESSAGES = 5000);
	~MessageSystem();

	bool pushMessage(IMessage *& message);
	bool pollMessage(IMessage *& message);

private:
	std::deque<IMessage*> m_active;
	std::deque<IMessage*> m_processed;

	const int m_MAX_MESSAGES;

	void cleanup();
	void processMessage(IMessage *& message);
};


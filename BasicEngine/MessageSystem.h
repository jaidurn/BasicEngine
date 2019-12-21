#pragma once
//==========================================================================================
// File Name: MessageSystem.h
// Author: Brian Blackmon
// Date Created: 12/9/2019
// Purpose: 
// Handles reading and processing messages.
//==========================================================================================
#include "Messages.h"
#include <deque>

class MessageSystem
{
public:
	MessageSystem(const int MAX_MESSAGES = 5000);
	~MessageSystem();

	bool pushMessage(Message *& message);
	bool pollMessage(Message *& message);

private:
	std::deque<Message*> m_active;
	std::deque<Message*> m_processed;

	const int m_MAX_MESSAGES;

	void cleanup();
	void processMessage(Message *& message);
};


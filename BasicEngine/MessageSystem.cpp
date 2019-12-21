#include "MessageSystem.h"
#include <iostream>

// Message includes
#include "IMessage.h"

MessageSystem::MessageSystem(const int MAX_MESSAGES)
	:m_MAX_MESSAGES(MAX_MESSAGES)
{

}

MessageSystem::~MessageSystem()
{
	cleanup();
}

//=============================================================================
// Function: bool pushMessage(Message*&)
// Description:
// Attempts to push a message to the message queue.
// Parameters:
// Message*& message - The message to push to the queue.
// Output:
// bool
// Success - Returns true if the message was added to the queue.
// Failure - Returns false if the message was unable to be added to the queue.
//=============================================================================
bool MessageSystem::pushMessage(Message *& message)
{
	bool success = false;

	if ((int)m_active.size() < m_MAX_MESSAGES)
	{
		m_active.push_back(message);
		success = true;
	}

	return success;
}

//=============================================================================
// Function: bool pollMessage(Message *&)
// Description:
// Polls the system for an unprocessed message.
// Parameters:
// Message *& message - The message to fill with message data.
// Output:
// bool
// Success - Returns true when a message is accessed.
// Failure - Returns false when a message isn't found.
//=============================================================================
bool MessageSystem::pollMessage(Message *& message)
{
	bool success = false;

	if (0 < m_active.size())
	{
		message = m_active.front();

		if (message != nullptr)
		{
			m_active[0] = nullptr;
			m_active.pop_front();

			processMessage(message);

			success = true;
		}
	}

	return success;
}

//=============================================================================
// Function: void cleanup()
// Description:
// Cleans up all of the messages in the deques and frees up the 
// memory.
//=============================================================================
void MessageSystem::cleanup()
{
	for (unsigned int i = 0; i < m_active.size(); i++)
	{
		delete m_active[i];
		m_active[i] = nullptr;
	}

	for (unsigned int i = 0; i < m_processed.size(); i++)
	{
		delete m_processed[i];
		m_processed[i] = nullptr;
	}

	m_active.resize(0);
	m_processed.resize(0);
}

//=============================================================================
// Function: void processMessage(Message *&)
// Description:
// Adds a message to the processed message deque.
// Parameters:
// Message *& message - The message to add.
//=============================================================================
void MessageSystem::processMessage(Message *& message)
{
	if (m_MAX_MESSAGES <= (int)m_processed.size())
	{
		delete m_processed[0];
		m_processed[0] = nullptr;
		m_processed.pop_front();
	}

	m_processed.push_back(message);
}
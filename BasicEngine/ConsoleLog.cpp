#include "ConsoleLog.h"
#include <time.h>

ConsoleLog::ConsoleLog()
{
}

ConsoleLog::~ConsoleLog()
{
}

//=============================================================================
// Function: void log(const string) const
// Description:
// Logs the message to the console.
// Parameters:
// const string message - The message to send to the console.
//=============================================================================
void ConsoleLog::log(const string message) const
{
	time_t tempTime = time(NULL);

	struct tm currentTime;

	localtime_s(&currentTime, &tempTime);

	std::cout << "["
		<< currentTime.tm_hour
		<< ":"
		<< currentTime.tm_min
		<< ":"
		<< currentTime.tm_sec
		<< "] ";

	std::cout << message
		<< std::endl;
}
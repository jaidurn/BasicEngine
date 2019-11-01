#include "FileLog.h"
#include <time.h>

FileLog::FileLog(string logPath)
	:m_logPath(logPath)
{
}

FileLog::~FileLog()
{
}

//=============================================================================
// Function: void log(const string)
// Description:
// Logs the message to the log file.
// Parameters:
// const string message - The string to log.
//=============================================================================
void FileLog::log(const string message) const
{
	if (m_logPath != "")
	{
		std::ofstream fout;

		fout.open(m_logPath, std::ios::app);

		if (fout.is_open())
		{
			time_t tempTime;
			struct tm currentTime;

			tempTime = time(NULL);

			localtime_s(&currentTime, &tempTime);

			fout << "["
				<< currentTime.tm_hour
				<< ":"
				<< currentTime.tm_min
				<< ":"
				<< currentTime.tm_sec
				<< "] "
				<< message
				<< std::endl;
		}

		fout.close();
	}
}

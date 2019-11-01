#include "LogLocator.h"
#include "ILog.h"

NullLog LogLocator::m_nullLog;
ILog* LogLocator::m_log = &m_nullLog;

LogLocator::LogLocator()
{
}

LogLocator::~LogLocator()
{
}

//=============================================================================
// Function: ILog& getLog()
// Description:
// Gets the currently stored log object.
// Output:
// ILog&
// Returns the current Log object.
//=============================================================================
ILog& LogLocator::getLog()
{
	return *m_log;
}

//=============================================================================
// Function: void setLog(ILog*)
// Description:
// Sets the current log to the provided log.
// Parameters:
// ILog *log - The log to set as our current log.
//=============================================================================
void LogLocator::setLog(ILog *log)
{
	if (log)
	{
		m_log = log;
	}
}
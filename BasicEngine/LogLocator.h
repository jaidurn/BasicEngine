#pragma once
//=========================================================================================
// File Name: LogLocator.h
// Author: Brian Blackmon
// Date Created: 10/16/2019
// Purpose: 
// Holds the location of a log, allowing other classes to find and use it.
//==========================================================================================
#include "NullLog.h"

class ILog;

class LogLocator
{
public:
	LogLocator();
	~LogLocator();

	static ILog& getLog();
	static void setLog(ILog *log);

private:
	static ILog *m_log;
	static NullLog m_nullLog;
};


#pragma once
//==========================================================================================
// File Name: FileLog.h
// Author: Brian Blackmon
// Date Created: 10/16/2019
// Purpose: 
// Logs messages to a file and saves the file.
//==========================================================================================
#include "ILog.h"
#include <fstream>
class FileLog :	public ILog
{
public:
	FileLog(string logPath);
	virtual ~FileLog();

	virtual void log(const string message) const;

private:
	string m_logPath;
};


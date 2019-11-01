#pragma once
//==========================================================================================
// File Name: ILog.h
// Author: Brian Blackmon
// Date Created: 10/16/2019
// Purpose: 
// Logs errors to the proper place based on settings.
//==========================================================================================
#include <string>

typedef std::string string;

class ILog
{
public:
	ILog() {}
	virtual ~ILog() {}
	
	virtual void log(const string message) const = 0;
};


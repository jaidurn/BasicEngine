#pragma once
//==========================================================================================
// File Name: ConsoleLog.h
// Author: Brian Blackmon
// Date Created: 10/16/2019
// Purpose: 
// Extends the Log class and allows for output to go to the console.
//==========================================================================================
#include <iostream>
#include "ILog.h"
class ConsoleLog : public ILog
{
public:
	ConsoleLog();
	virtual ~ConsoleLog();

	virtual void log(const string message) const;
};


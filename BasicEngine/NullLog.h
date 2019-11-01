#pragma once
//==========================================================================================
// File Name: 
// Author: 
// Date Created: 10/16/2019
// Purpose: 
// A log that does absolutely nothing.
//==========================================================================================
#include "ILog.h"
class NullLog :
	public ILog
{
public:
	NullLog() {}
	virtual ~NullLog() {}

	virtual void log(const string message) const {}
};


#pragma once
//=============================================================================
// File Name: LocatorIO.h
// Author: Brian Blackmon
// Date Created: 12/21/2019
// Purpose: 
// A locator tool for in and out.
//=============================================================================
#include "SettingIO.h"

class LocatorIO
{
public:
	~LocatorIO() {}

	static SettingIO* getIO();
	static void setIO(SettingIO* io);

private:
	LocatorIO() {}

	static SettingIO* m_io;
};

SettingIO* LocatorIO::m_io = NULL;

SettingIO* LocatorIO::getIO()
{
	if (!m_io)
	{
		m_io = new SettingIO("Resources/", ".dat");
	}

	return m_io;
}

void LocatorIO::setIO(SettingIO* io)
{
	if (io && m_io != io)
	{
		if (m_io)
		{
			delete m_io;
		}

		m_io = io;
	}
}
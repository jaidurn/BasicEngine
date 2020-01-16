#pragma once
//=============================================================================
// File Name: BinaryIO.h
// Author: Brian Blackmon
// Date Created: 12/26/2019
// Purpose: 
// Handles reading and writing to files in binary.
//=============================================================================
#include "IO.h"
#include "BB_File.h"

class BinaryIO : public I_IO
{
public:
	BinaryIO();
	virtual ~BinaryIO();

	BB_File* open(string filePath, const bool parse = false);
	bool save(BB_File* file, const bool parse = false);

private:	
	// TODO: Impliment these later.
	static const int m_KEY_LENGTH = 10;
	const char m_KEY[m_KEY_LENGTH] = "DankMemes";

	void parseFile(BB_File* file);
};
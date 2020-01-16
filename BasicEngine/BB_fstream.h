#pragma once
//=============================================================================
// File Name: BB_fstream.h
// Author: Brian Blackmon
// Date Created: 12/26/2019
// Purpose: 
// A stream object for manipulating BB_File objects.
//=============================================================================
#include "BB_File.h"

class BB_fstream
{
public:
	enum BB_filePos
	{
		BB_fileBeg,
		BB_fileCur,
		BB_fileEnd
	};

	enum BB_fileState : unsigned int
	{
		BB_fstateGood = 0x00,
		BB_fstateEOF = 0x01,
		BB_fstateFail = 0x02,
		BB_fstateBad = 0x04
	};

	BB_fstream();
	BB_fstream(BB_File* file);

	~BB_fstream();

	bool open(BB_File* file);
	bool isOpen() const;
	bool atEOF() const;
	long tell() const;
	void close();

	// Error checking
	bool goodState() const;
	bool failState() const;
	bool badState() const;
	bool eofState() const;
	void clearFlags();

	bool write(const char* data, const long size);
	bool read(char* data, const long size);

	bool readUntil(char* data,
		const long maxSize, 
		const char deliminator);

	long find(const char* data, const long size, const long startPos = 0);
	
	long search(const char* data,
		const long size,
		const long startPos = 0,
		const long endPos = -1);

	bool replace(const char* replaceData,
		const long replaceSize,
		const char* insertData,
		const long insertSize,
		const long pos = 0);

	bool remove(const char* data, 
		const long size, 
		const long startPos = 0);

	void seek(const long amount, BB_filePos position = BB_fileCur);

	// Basic type overloads.
	BB_fstream& operator<<(const int value);
	BB_fstream& operator<<(const float value);
	BB_fstream& operator<<(const double value);
	BB_fstream& operator<<(const bool value);
	BB_fstream& operator<<(const string value);

	BB_fstream& operator>>(int& value);
	BB_fstream& operator>>(float& value);
	BB_fstream& operator>>(double& value);
	BB_fstream& operator>>(bool& value);
	BB_fstream& operator>>(string& value);

	explicit operator bool() const;

private:
	BB_File* m_file;
	long m_pos;
	unsigned int m_stateFlags;

	void insert(const char* data, 
		const long size, 
		const long pos = 0);
};
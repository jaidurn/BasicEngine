#pragma once
//=============================================================================
// File Name: BB_File.h
// Author: Brian Blackmon
// Date Created: 12/26/2019
// Purpose: 
// Wraps a basic file with information.
//=============================================================================
#include <string>
#include <fstream>

typedef std::string string;

struct BB_File
{
	BB_File()
		:m_data(nullptr),
		m_size(0),
		m_filePath("")
	{

	}

	~BB_File()
	{
		delete[] m_data;
		m_data = nullptr;
		m_size = 0;
		m_filePath = "";
	}

	char* m_data;
	long m_size;
	string m_filePath;
};

std::ofstream& operator<<(std::ofstream& fout, BB_File* file);
std::ifstream& operator>>(std::ifstream& fin, BB_File* file);
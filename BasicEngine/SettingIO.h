#pragma once
//==========================================================================================
// File Name: SettingIO.h
// Author: Brian Blackmon
// Date Created: 10/21/2019
// Purpose: 
// Handles loading and saving of settings.
//==========================================================================================
#include <string>
#include <fstream>
#include <cstdio>
#include <sstream>

typedef std::string string;
typedef std::ios::_Openmode OpenMode;

class SettingIO
{
public:
	SettingIO(const string rootPath,
		const string fileType);
	~SettingIO();

	const string getRootPath() const;
	const string getFileType() const;
	const string getCurrentFile() const;

	//=============================================================================
	// Function: bool getSetting(const string, T&)
	// Description:
	// Gets the value of the specified setting name.
	// If the setting isn't found, it sets the setting to false.
	// Parameters:
	// const string settingName - The name of the setting to get.	
	// T& settingValue - The value of the setting.
	// Output:
	// bool
	// On success returns true.
	// On failure returns false.
	//=============================================================================
	template <typename T>
    bool getSetting(const string settingName, 
		T& settingValue)
	{
		bool settingFound = false;

		if (m_stream.is_open() && 
			getCurrentMode() == std::ios::in)
		{
			std::streamoff currentPos = m_stream.tellg();
			string temp = "";

			if (currentPos < 0)
			{
				m_stream.seekg(0, std::ios::beg);
				m_stream.clear();
				currentPos = 0;
			}

			while (m_stream >> temp)
			{
				if (temp == settingName)
				{
					m_stream >> settingValue;
					settingFound = true;
					break;
				}
			}

			if (!settingFound && currentPos != 0)
			{
				m_stream.seekg(0, std::ios::beg);
				m_stream.clear();

				while (m_stream.tellg() < currentPos)
				{
					m_stream >> temp;

					if (temp == settingName)
					{
						m_stream >> settingValue;
						settingFound = true;
						break;
					}
				}
			}
		}

		return settingFound;
	}


	template <typename T>
	bool saveSetting(const string settingName,
		const T settingValue)
	{
		bool settingSaved = false;

		if (m_currentFile != "")
		{
			if (!m_stream.is_open())
			{
				openFile(m_currentFile, std::ios::in);
			}
			else
			{
				m_stream.seekg(0, std::ios::beg);
			}

			if (m_stream.is_open())
			{
				std::fstream fout;

				string tempFile = m_rootPath + m_currentFile;
				tempFile += "temp" + m_fileType;

				fout.open(tempFile,	std::ios::out);

				string temp = "";

				if (fout.is_open())
				{
					while (std::getline(m_stream, temp))
					{
						if (temp.find(settingName) == 0)
						{
							fout << settingName
								<< " "
								<< settingValue
								<< std::endl;

							settingSaved = true;
						}
						else
						{
							fout << temp
								<< std::endl;
						}
					}

					if (!settingSaved)
					{
						fout << settingName
							<< " "
							<< settingValue
							<< std::endl;
					}

					string fileTemp = m_rootPath + m_currentFile + m_fileType;
					string currentTemp = m_rootPath + m_currentFile + m_fileType;

					m_stream.close();
					fout.close();

					remove(fileTemp.c_str());
					rename(tempFile.c_str(), currentTemp.c_str());

					openFile(m_currentFile, std::ios::in);
				}
			}
		}

		return settingSaved;
	}

	void setRootPath(const string root);
	void setFileType(const string type);

	void openFile(const string fileName,
		const OpenMode openMode);
	bool isFileOpen() const;
	void closeFile();

private:
	string m_rootPath;
	string m_fileType;
	string m_currentFile;
	std::fstream m_stream;

	OpenMode getCurrentMode() const;
};
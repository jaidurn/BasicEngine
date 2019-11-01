#include "SettingIO.h"

SettingIO::SettingIO(const string rootPath,
	const string fileType)
	:m_rootPath(rootPath),
	m_fileType(fileType),
	m_currentFile("")
{
	if (m_fileType == "")
	{
		m_fileType = ".dat";
	}
}

SettingIO::~SettingIO()
{
	//closeFile();
}

//=============================================================================
// Function: const string getRootPath() const
// Description:
// Gets the current root path the settingIO is using.
// Output:
// const string
// Returns the string with the current root path.
//=============================================================================
const string SettingIO::getRootPath() const
{
	return m_rootPath;
}

//=============================================================================
// Function: const string getFileType() const
// Description:
// Gets the current file type the settingIO is capable of loading.
// Output:
// const string
// Returns the string with the current file type.
//=============================================================================
const string SettingIO::getFileType() const
{
	return m_fileType;
}

//=============================================================================
// Function: const string getCurrentFile() const
// Description:
// Gets the currently opened file name.
// Output:
// const string
// Returns the currently opened file name. If there is
// no open file, it sets the file name to "".
//=============================================================================
const string SettingIO::getCurrentFile() const
{
	return m_currentFile;
}

//=============================================================================
// Function: void setRootPath(const string)
// Description:
// Sets the root path to use for the setting loading.
// Parameters:
// const string root - The new root path.
//=============================================================================
void SettingIO::setRootPath(const string root)
{
	m_rootPath = root;
}

//=============================================================================
// Function: void setFileType(const string)
// Description:
// Sets the file type to the new type if it isn't empty.
// Parameters:
// const string type - The new file type to use.
//=============================================================================
void SettingIO::setFileType(const string type)
{
	if (type != "")
	{
		m_fileType = type;
	}
}

//=============================================================================
// Function: void openFile(const string, const OpenMode)
// Description:
// Opens the file with the name.
// Parameters:
// const string fileName - The file name to open.
// const OpenMode openMode - The open mode to use.
//=============================================================================
void SettingIO::openFile(const string fileName,
	const OpenMode openMode)
{
	if (fileName != "")
	{
		if (m_stream.is_open())
		{
			m_stream.close();
		}

		m_stream.open(m_rootPath + fileName + m_fileType, openMode);

		if (m_stream.is_open())
		{
			m_currentFile = fileName;
		}
	}
}

//=============================================================================
// Function: bool isFileOpen() const
// Description:
// Checks to see if there's a file currently open.
// Output:
// bool
// Returns true if there's a file open.
// Returns false if there isn't a file open.
//=============================================================================
bool SettingIO::isFileOpen() const
{
	return m_stream.is_open();
}

//=============================================================================
// Function: void closeFile()
// Description:
// Closes the currently open file if there is one.
//=============================================================================
void SettingIO::closeFile()
{
	if (isFileOpen())
	{
		m_stream.close();
		m_currentFile = "";
	}
}

//=============================================================================
// Function: OpenMode getCurrentMode() const
// Description:
// Gets the open mode of the current stream.
// Output:
// OpenMode
// Returns the open mode of the current stream.
//=============================================================================
OpenMode SettingIO::getCurrentMode() const
{
	OpenMode mode = std::ios::in;

	return mode;
}
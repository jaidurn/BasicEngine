#include "BB_fstream.h"

// TODO: Add error logging for better debugging.

BB_fstream::BB_fstream()
	:m_file(nullptr),
	m_pos(-1)
{

}

BB_fstream::BB_fstream(BB_File* file)
	:m_file(file),
	m_pos(0)
{

}

BB_fstream::~BB_fstream()
{
	close();
}

//=============================================================================
// Function: bool open(BB_File*)
// Description: 
// Trys to open the file in the stream. Returns false if
// another file is already open.
// Parameters: 
// BB_File* file - The file to open.
// Output: 
// bool
// On success - Returns true if the file opened.
// On failure - Returns false if the file didn't open.
//=============================================================================
bool BB_fstream::open(BB_File* file)
{
	if (m_file == nullptr && file != nullptr)
	{
		m_file = file;
		m_pos = 0;

		return true;
	}

	return false;
}

//=============================================================================
// Function: bool isOpen() const
// Description: 
// Checks to see if the stream currently has a file loaded.
// Output: 
// bool
// On success - Returns true if file is open.
// On failure - Returns false if no file is loaded.
//=============================================================================
bool BB_fstream::isOpen() const
{
	if (m_file != nullptr)
	{
		return true;
	}

	return false;
}

//=============================================================================
// Function: bool atEOF() const
// Description: 
// Checks to see if the stream is at the end of the current file.
// Output: 
// bool
// Returns true if the current position is at the end of the file.
// Also returns true if there's no open file.
// Returns false if the current position isn't at end of file.
//=============================================================================
bool BB_fstream::atEOF() const
{
	if (isOpen())
	{
		if (m_pos < m_file->m_size - 1)
		{
			return false;
		}
	}

	return true;
}

//=============================================================================
// Function: 
// Description: 
// Gets the position of the current stream. Returns -1 if there is no open
// stream.
// Output: 
// long
// Returns the current stream position.
//=============================================================================
long BB_fstream::tell() const
{	return m_pos;
}

//=============================================================================
// Function: void close()
// Description: 
// Closes any currently open file.
// WARNING: DOES NOT FREE MEMORY HELD WITH FILE.
//=============================================================================
void BB_fstream::close()
{
	m_file = nullptr;
	m_pos = -1;
}

//=============================================================================
// Function: bool goodState() const
// Description: 
// Checks the state flags of the stream and sees if they're good.
// Output: 
// bool
// Returns true if the state is good.
// Returns false if any other state is set.
//=============================================================================
bool BB_fstream::goodState() const
{
	if (m_stateFlags == BB_fstateGood)
	{
		return true;
	}

	return false;
}

//=============================================================================
// Function: bool failState() const
// Description: 
// Checks to see if the stream state is in a fail state.
// The fail state is thrown if either the file isn't open
// or if the file failed to be read to or from.
// Output: 
// bool
// Returns true if the fail state bit was set.
// Returns false if the fail state bit was 0.
//=============================================================================
bool BB_fstream::failState() const
{
	if (m_stateFlags & BB_fstateFail)
	{
		return true;
	}

	return false;
}

//=============================================================================
// Function: bool badState() const
// Description: 
// Checks to see if the stream is in a bad state.
// The bad state is for failures during reading/writing to a file.
// NOTE: The fail state will be set when this is set, too.
// Output: 
// bool 
// Returns true if the bad state bit is set.
// Returns false if the bad state is 0.
//=============================================================================
bool BB_fstream::badState() const
{
	if (m_stateFlags & BB_fstateBad)
	{
		return true;
	}

	return false;
}

//=============================================================================
// Function: bool eofState() const
// Description: 
// Checks to see if the current state of the stream 
// is at the end of a file. This only matters for
// reading from a file.
// Output: 
// bool
// Returns true if the end of file bit is set.
// Returns false if it's 0.
//=============================================================================
bool BB_fstream::eofState() const
{
	if (m_stateFlags & BB_fstateEOF)
	{
		return true;
	}

	return false;
}

//=============================================================================
// Function: void clearFlags() 
// Description: 
// Clears all of the flags and sets the state to good.
//=============================================================================
void BB_fstream::clearFlags()
{
	m_stateFlags = BB_fstateGood;
}

//=============================================================================
// Function: bool write(const char*, const long)
// Description: 
// Writes the data to the file at the current position.
// Parameters: 
// const char* data - The data to write.
// const long size - The size of the data.
// Output: 
// bool
// On success - Returns true on successful write.
// On failure - Returns false on failure to write completely.
//=============================================================================
bool BB_fstream::write(const char* data, const long size)
{
	if (isOpen() && (goodState() || eofState()))
	{
		if (data != nullptr && 0 < size)
		{
			long biggerPos = 0;
			long biggerSize = m_file->m_size + size;
			char* biggerData = new char[biggerSize] {};

			if (biggerData != nullptr)
			{
				if (atEOF())
				{
					for (long i = 0; i < m_file->m_size; i++)
					{
						biggerData[biggerPos] = m_file->m_data[i];
						biggerPos++;
					}

					for (long i = 0; i < size; i++)
					{
						biggerData[biggerPos] = *(data + i);
						biggerPos++;
					}
				}
				else
				{
					for (long i = 0; i < m_file->m_size; i++)
					{
						if (i == m_pos)
						{
							for (long j = 0; j < size; j++)
							{
								biggerData[biggerPos] = *(data + j);
								biggerPos++;
							}
						}

						biggerData[biggerPos] = m_file->m_data[i];
						biggerPos++;
					}
				}

				delete[] m_file->m_data;

				m_file->m_data = biggerData;
				m_file->m_size = biggerSize;
				m_pos += size;
			}
			else
			{
				m_stateFlags |= BB_fstateFail;
				m_stateFlags |= BB_fstateBad;

				return false;
			}

			return true;
		}
	}
	else
	{
		m_stateFlags |= BB_fstateFail;
	}

	return false;
}

//=============================================================================
// Function: bool read(char*, const long)
// Description: 
// Reads in data from the file into the data pointer. Returns
// an error if it fails to read all of the data.
//
// NOTE: If data is read successfully, the position is moved forward.
// If the data fails to read, the position stays where it's at.
//
// Parameters: 
// char* data - The data to be read into.
// const long size - The size of the data to be read.
// Output:
// On success - Returns true if all the data is read.
// On failure - Returns false if no data or partial data is read.
//=============================================================================
bool BB_fstream::read(char* data, const long size)
{
	if (goodState())
	{
		if (isOpen())
		{
			if (!atEOF())
			{
				if (m_pos + size <= m_file->m_size)
				{
					for (long i = 0; i < size; i++)
					{
						*(data + i) = m_file->m_data[m_pos + i];
					}

					m_pos += size;

					return true;
				}
				else
				{
					// The file is too short to read in all the data.
					m_stateFlags |= BB_fstateFail;
					m_stateFlags |= BB_fstateBad;
				}
			}
			else
			{
				m_stateFlags |= BB_fstateEOF;
			}
		}
		else
		{
			m_stateFlags |= BB_fstateFail;
		}
	}

	return false;
}

//=============================================================================
// Function: bool readUntil(char*, const long, const char)
// Description: 
// Reads the file until the specified deliminator is found.
// Parameters: 
// char* data - The data to read into.
// const long maxSize - The size of the data.
// const char deliminator - The deliminating character.
// Output: 
// bool
// Returns true if the data was able to be read.
// Returns false if the data was too large.
//=============================================================================
bool BB_fstream::readUntil(char* data, 
	const long maxSize,
	const char deliminator)
{
	if (isOpen())
	{
		long delimIndex = find(&deliminator, sizeof(deliminator), m_pos);

		if (-1 < delimIndex)
		{
			long size = delimIndex - m_pos;

			if (size <= maxSize)
			{
				if (read(data, size))
				{
					return true;
				}
			}

		}
	}

	return false;
}

//=============================================================================
// Function: long find(const char*, const long, const long)
// Description: 
// Finds the first instance of the data from the starting pos.
// Parameters: 
// const char* data - The data to find.
// const long size - The size of the data.
// const long startPos - The starting position to search from.
// Output: 
// long
// On success - Returns the start index of the found data.
// On failure - Returns -1.
//=============================================================================
long BB_fstream::find(const char* data,
	const long size, 
	const long startPos)
{
	if (isOpen())
	{
		if (data != nullptr && 0 < size)
		{
			long workingPos = startPos;

			if (workingPos < 0)
			{
				workingPos = 0;
			}
			else if (m_file->m_size <= workingPos)
			{
				workingPos = m_file->m_size - 1;
			}

			long dataIndex = 0;

			for (long i = workingPos; i < m_file->m_size; i++)
			{
				if (m_file->m_data[i] == *(data + dataIndex))
				{
					dataIndex++;
				}
				else
				{
					dataIndex = 0;
				}

				if (dataIndex == size)
				{
					return i + 1 - dataIndex;
				}
			}
		}
	}

	return -1;
}

//=============================================================================
// Function: long search(
// const char*, const long, const long, const long)
// Description: 
// Searches between two indexes for the specified data and returns
// the found index if possible.
// Parameters: 
// const char* data - The data to search for.
// const long size - The size of the data to search for.
// const long startPos - The starting point of the search.
// const long endPos - The end point of the search. -1 for the end.
// Output: 
// long
// On success - Returns the starting index of the found data.
// On failure - Returns -1.
//=============================================================================
long BB_fstream::search(const char* data,
	const long size,
	const long startPos,
	const long endPos)
{
	if (isOpen())
	{
		long workingEndPos = endPos;
		long workingStartPos = startPos;

		if (workingEndPos == -1 || m_file->m_size <= workingEndPos)
		{
			workingEndPos = m_file->m_size - 1;
		}

		if (workingStartPos < 0)
		{
			workingStartPos = 0;
		}
		else if (m_file->m_size <= workingStartPos)
		{
			workingStartPos = m_file->m_size - 1;
		}

		if (workingStartPos < workingEndPos)
		{
			long dataIndex = 0;

			for (long i = workingStartPos; i < workingEndPos; i++)
			{
				if (m_file->m_data[i] == *(data + dataIndex))
				{
					dataIndex++;
				}
				else
				{
					dataIndex = 0;
				}

				if (dataIndex == size)
				{
					return i + 1 - dataIndex;
				}
			}
		}
	}

	return -1;
}

//=============================================================================
// Function: bool replace(
// const char*, const long, const char*, const long, const long)
// Description: 
// Finds the first instance of the replace data from the 
// starting position, removes it, and then inserts the new data.
// Parameters: 
// const char* replaceData - The data to replace.
// const long replaceSize - The size of the data to replace.
// const char* insertData - The data to insert.
// const long insertSize - The size of the data to insert.
// const long pos - The starting position to search from.
// Output: 
// bool
// Returns true if the data is found and replaced.
// Returns false if the data isn't found.
//=============================================================================
bool BB_fstream::replace(const char* replaceData,
	const long replaceSize,
	const char* insertData,
	const long insertSize,
	const long pos)
{
	if (isOpen())
	{
		if (replaceData != nullptr && 0 < replaceSize &&
			insertData != nullptr && 0 < insertSize)
		{
			long workingPos = pos;

			if (workingPos < 0)
			{
				workingPos = 0;
			}
			else if(m_file->m_size <= workingPos)
			{
				return false;
			}

			long findIndex = find(replaceData, replaceSize, workingPos);

			if (-1 < findIndex)
			{
				remove(replaceData, replaceSize, workingPos);

				insert(insertData, insertSize, workingPos);

				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Function: void remove(const char*, const long, const long)
// Description: 
// Looks for the data from the start position and removes it if found.
// Parameters: 
// const char* data - The data to remove.
// const long size - The size of the data to remove.
// const long startPos - The position to start searching from.
// Output:
// Returns true if remove was successful.
// Returns false if the remove couldn't happen.
//=============================================================================
bool BB_fstream::remove(const char* data,
	const long size,
	const long startPos)
{
	if (isOpen())
	{
		long workingPos = startPos;

		if (workingPos < 0)
		{
			workingPos = 0;
		}
		else if (m_file->m_size <= workingPos)
		{
			return false;
		}

		long findIndex = find(data, size, workingPos);

		if (-1 < findIndex)
		{
			long smallerSize = m_file->m_size - size;
			char* smallerData = new char[smallerSize] {};

			if (smallerData != nullptr)
			{
				long smallerIndex = 0;

				for (long i = 0; i < m_file->m_size; i++)
				{
					if (i < findIndex || findIndex + size <= i)
					{
						smallerData[smallerIndex] = m_file->m_data[i];
						smallerIndex++;
					}
				}

				delete[] m_file->m_data;

				m_file->m_data = smallerData;
				m_file->m_size = smallerSize;

				if (smallerSize <= m_pos)
				{
					m_pos = smallerSize - 1;
				}

				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Function: void seek(const long, BB_filePos)
// Description: 
// Seeks to the new position from the specified point.
// Parameters: 
// const long amount - The amount of bytes to seek. 
//					   Can be negative for seeking in reverse.
// BB_filePos position - The position to seek from.
//		BB_fileBeg - Seeks from the begining of the file.
//		BB_fileCur - Seeks from the current point in the file.
//		BB_fileEnd - Seeks from the end of the file.
//=============================================================================
void BB_fstream::seek(const long amount, BB_filePos position)
{
	if (isOpen())
	{
		long newPos = 0;

		switch (position)
		{
		case BB_fileCur:
		{
			newPos = m_pos + amount;
			break;
		}
		case BB_fileBeg:
		{
			newPos += amount;
			break;
		}
		case BB_fileEnd:
		{
			newPos = (m_file->m_size - 1) + amount;
			break;
		}
		}

		// Fix the position.

		if (newPos < 0)
		{
			newPos = 0;
		}
		else if (m_file->m_size <= newPos)
		{
			newPos = m_file->m_size - 1;
		}

		m_pos = newPos;
	}
}

// Write overloads.
// TODO: Add error logging.
BB_fstream& BB_fstream::operator<<(const int value)
{
	if (isOpen())
	{
		write((char*)&value, sizeof(value));
	}

	return *this;
}

BB_fstream& BB_fstream::operator<<(const float value)
{
	if (isOpen())
	{
		write((char*)&value, sizeof(value));
	}

	return *this;
}

BB_fstream& BB_fstream::operator<<(const double value)
{
	if (isOpen())
	{
		write((char*)&value, sizeof(value));
	}

	return *this;
}

BB_fstream& BB_fstream::operator<<(const bool value)
{
	if (isOpen())
	{
		write((char*)&value, sizeof(value));
	}

	return *this;
}

BB_fstream& BB_fstream::operator<<(const string value)
{
	if (isOpen())
	{
		char end = '\0';
		write(value.c_str(), value.length());
		write(&end, sizeof(end));
	}

	return *this;
}

// Read overloads.
BB_fstream& BB_fstream::operator>>(int& value)
{
	if (isOpen())
	{
		read((char*)&value, sizeof(value));
	}

	return *this;
}

BB_fstream& BB_fstream::operator>>(float& value)
{
	if (isOpen())
	{
		read((char*)&value, sizeof(value));
	}

	return *this;
}

BB_fstream& BB_fstream::operator>>(double& value)
{
	if (isOpen())
	{
		read((char*)&value, sizeof(value));
	}

	return *this;
}

BB_fstream& BB_fstream::operator>>(bool& value)
{
	if (isOpen())
	{
		read((char*)&value, sizeof(value));
	}

	return *this;
}

//=============================================================================
// Function: operator bool() const
// Description: 
// Overloads the bool conversion operator allowing for the 
// state of the stream to be used as a check.
// Output: 
// Returns true if the stream is good.
// Returns false if there was an error thrown.
//=============================================================================
BB_fstream::operator bool() const
{
	return goodState();
}

//=============================================================================
// Function: BB_fstream& operator>>(string& value)
// Description: 
// This function needs a little more information to use than the
// rest of them.
// Termination characters to strings:
// '\0'
// ' '
// '\n'
// '\t'
// '\r'
// The priority is whichever comes soonest.
// The stream moves to the point after the deliminator.
// Parameters: 
// string& value - The string to set.
// Output: 
// BB_fstream&
// Returns the modified stream.
//=============================================================================
BB_fstream& BB_fstream::operator>>(string& value)
{
	if (!atEOF())
	{
		char space = ' ';
		char nullChar = '\0';
		char newLine = '\n';
		char tab = '	';
		char carReturn = '\r';

		long nullPos = find(&nullChar, sizeof(nullChar), tell());
		long spacePos = find(&space, sizeof(space), tell());
		long newLinePos = find(&newLine, sizeof(newLine), tell());
		long tabPos = find(&tab, sizeof(tab), tell());
		long carReturnPos = find(&carReturn, sizeof(carReturn), tell());
		long closestPos = nullPos;

		if (-1 < spacePos)
		{
			if (closestPos == -1 || spacePos < closestPos)
			{
				closestPos = spacePos;
			}
		}

		if (-1 < newLinePos)
		{
			if (closestPos == -1 || newLinePos < closestPos)
			{
				closestPos = newLinePos;
			}
		}

		if (-1 < tabPos)
		{
			if (closestPos == -1 || tabPos < closestPos)
			{
				closestPos = tabPos;
			}
		}

		if (-1 < carReturnPos)
		{
			if (closestPos == -1 || carReturnPos < closestPos)
			{
				closestPos = carReturnPos;
			}
		}

		long length = closestPos - tell();

 		if (closestPos == -1)
		{
			length = (m_file->m_size - 1) - tell();
		}

		char* str = new char[length + 1] {};

		read(str, length);
		seek(1, BB_fileCur);

		value = str;

		delete[] str;
	}

	return *this;
}

//=============================================================================
// Function: void insert(const char*, const long, const long)
// Description: 
// Inserts the data at the specified point.
// Parameters: 
// const char* data - The data to insert.
// const long size - The size of the data.
// const long pos - The position of where to insert the data.
//=============================================================================
void BB_fstream::insert(const char* data,
	const long size,
	const long pos)
{
	if (isOpen())
	{
		if (data != nullptr && 0 < size)
		{
			long workingPos = pos;

			if (workingPos < 0)
			{
				workingPos = 0;
			}
			else if (m_file->m_size <= workingPos)
			{
				workingPos = m_file->m_size - 1;
			}

			long biggerSize = m_file->m_size + size;
			char* biggerData = new char[biggerSize] {};

			if (biggerData)
			{
				long biggerPos = 0;

				for (long i = 0; i < m_file->m_size; i++)
				{
					if (i == workingPos)
					{
						for (long j = 0; j < size; j++)
						{
							biggerData[biggerPos] = *(data + j);
							biggerPos++;
						}
					}

					biggerData[biggerPos] = m_file->m_data[i];
					biggerPos++;
				}

				delete[] m_file->m_data;

				m_file->m_data = biggerData;
				m_file->m_size = biggerSize;
			}
		}
	}
}

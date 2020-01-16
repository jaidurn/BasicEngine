#include "BinaryIO.h"

BinaryIO::BinaryIO()
{

}

BinaryIO::~BinaryIO()
{

}

//=============================================================================
// Function: BB_File* open(string, const bool)
// Description: 
// Loads in a binary file and saves it into a BB_File object.
// Parameters: 
// string filePath - The path of the file to load.
// const bool parse - If true, parses the file through the key.
// Output: 
// BB_File*
// On success - Returns a pointer to the found BB_File.
// On failure - Returns a nullptr.
//=============================================================================
BB_File* BinaryIO::open(string filePath, const bool parse)
{
	std::ifstream fin(filePath, std::ios::binary | std::ios::in);

	if (fin.is_open())
	{
		BB_File* file = new BB_File;

		std::ifstream::pos_type begin = fin.tellg();
		
		fin.seekg(0, std::ios::end);

		std::ifstream::pos_type end = fin.tellg();

		fin.seekg(0, std::ios::beg);

		file->m_size = end - begin;
		file->m_data = new char[file->m_size] {};

		if (fin.read(file->m_data, file->m_size))
		{
			file->m_filePath = filePath;

			fin.close();

			if (parse)
			{
				parseFile(file);
			}

			return file;
		}
		else
		{
			delete file;
			fin.close();
		}
	}

	return nullptr;
}

//=============================================================================
// Function: bool save(BB_File*, const bool)
// Description: 
// Saves the file to the file path the file contains.
// Parameters: 
// BB_File* file - The file to save.
// const bool parse - If true, runs the file through the key.
// Output: 
// bool
// On success - Returns true if the file saved.
// On failure - Returns false if the file failed to save.
//=============================================================================
bool BinaryIO::save(BB_File* file, const bool parse)
{
	if (file != nullptr)
	{
		if (file->m_filePath != "")
		{
			std::ofstream fout(file->m_filePath, std::ios::binary);

			if (fout.is_open())
			{
				if (parse)
				{
					parseFile(file);
				}

				fout.write(file->m_data, file->m_size);

				fout.close();

				return true;
			}
		}
	}

	return false;
}

///=============================================================================
// Function: void parseFile(BB_File*)
// Description: 
// Either encrypts or decrypts the file using the key.
// Parameters: 
// BB_File* file - The file to parse with the key.
//=============================================================================
void BinaryIO::parseFile(BB_File* file)
{
	if (file != nullptr)
	{
		if (file->m_data != nullptr)
		{
			int keyPos = 0;

			for (long i = 0; i < file->m_size - 1; i++)
			{
				file->m_data[i] ^= m_KEY[keyPos];

				keyPos = (keyPos + 1) % m_KEY_LENGTH;
			}
		}
	}
}
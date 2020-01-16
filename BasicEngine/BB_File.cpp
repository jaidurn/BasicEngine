#include "BB_File.h"

std::ofstream& operator<< (std::ofstream& fout, BB_File* file)
{
	fout.write(file->m_data, file->m_size - 1);

	return fout;
}

std::ifstream& operator>>(std::ifstream& fin, BB_File* file)
{
	fin.read(file->m_data, file->m_size);

	return fin;
}
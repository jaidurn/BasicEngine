#include "EntityManager.h"

std::map<string, EntityData*> EntityManager::m_entityData;
std::map<int, string> EntityManager::m_entityTypes;
std::unordered_map<int, bool> EntityManager::m_entityFlags;

SettingIO EntityManager::m_io;

int EntityManager::m_currentID = 0;
string EntityManager::m_currentDataPath = "";

EntityManager::EntityManager()
{
	
}

EntityManager::~EntityManager()
{

}

//=============================================================================
// Function: bool loadEntitiesFromFile(string)
// Description:
// Clears the entity data currently being held and then loads in entities 
// from the file and adds them to the entity data map.
// Parameters:
// string filePath - The file path containing the entity data.
// Output:
// bool
// Success - Returns true if entity data is properly loaded from the file.
// Failure - Returns false if the entity data fails to load.
//=============================================================================
bool EntityManager::loadEntitiesFromFile(const string filePath)
{
	m_io.closeFile();
	m_io.openFile(filePath, std::ios::in);

	if (m_io.isFileOpen())
	{
		int entityDataCount = 0;

		m_io.getSetting("EntityCount", entityDataCount);

		std::vector<string> entityDataPaths;

		for (int i = 0; i < entityDataCount; i++)
		{
			string dataPath = "";

			if(m_io.getSetting(std::to_string(i), dataPath))
			{
				entityDataPaths.push_back(dataPath);
			}
		}

		for (unsigned int i = 0; i < entityDataPaths.size(); i++)
		{

		}
	}
}
#pragma once
//==========================================================================================
// File Name: EntityManager.h
// Author: Brian Blackmon
// Date Created: 12/10/2019
// Purpose: 
// A static class.
// Handles the loading, deletion, and storage of entity information.
//==========================================================================================
#include <string>
#include <map>
#include <unordered_map>
#include "EntityInfo.h"

typedef std::string string;

struct Message;
struct BB_File;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	static bool loadEntityFile(const string filepath);

	static bool loadEntitiesFromFile(const string filepath);
	static void clearEntityData();

//	static void processMessage(const Message* message);

//	static string getEntityType(const int entityID);
//	static EntityData getEntityData(const string entityType);

//	static bool entityExists(const int entityID);

private:
	static const int m_MAX_NAME_LENGTH = 512;

	static std::map<string, EntityInfo*> m_entityInfo;
	static std::map<int, string> m_entityTypes;
	static std::unordered_map<int, bool> m_entityFlags;

	static BB_File* m_entityFile;

	static int m_currentID;
	static string m_currentDataPath;

//	static int getNextFreeID();

//	static void removeUnusedData();

	static void loadEntityType(const string type);
};


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
#include "SettingIO.h"
#include "EntityData.h"

typedef std::string string;

struct Message;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	static bool loadEntitiesFromFile(const string filepath);
	static void clearEntityData();

	static void processMessage(const Message* message);

	static string getEntityType(const int entityID);
	static EntityData getEntityData(const string entityType);

	static bool entityExists(const int entityID);

private:
	static std::map<string, EntityData*> m_entityData;
	static std::map<int, string> m_entityTypes;
	static std::unordered_map<int, bool> m_entityFlags;

	// TODO: Remove IO from this class. Make it a standalone.
	static SettingIO m_io;

	static int m_currentID;
	static string m_currentDataPath;

	static int getNextFreeID();

	static void removeUnusedData();
};


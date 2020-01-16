#include "EntityManager.h"
#include "BinaryIO.h"
#include "BB_fstream.h"
#include "LogLocator.h"
#include "CollisionBox.h"
#include "Sprite.h"
#include "Animator.h"
#include "ResourceManager.h"

std::map<string, EntityInfo*> EntityManager::m_entityInfo;
std::map<int, string> EntityManager::m_entityTypes;
std::unordered_map<int, bool> EntityManager::m_entityFlags;

BB_File* EntityManager::m_entityFile = nullptr;

int EntityManager::m_currentID = 0;
string EntityManager::m_currentDataPath = "";


EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

//=============================================================================
// Function: bool loadEntityFile(const string)
// Description: 
// Loads in the entity file for use.
// Parameters: 
// const string filePath - The path to the entity data.
// Output: 
// bool
// On success - Returns true if entity file was loaded.
// On failure - Returns false if the entity file wasn't loaded.
//=============================================================================
bool EntityManager::loadEntityFile(const string filePath)
{
	if (!m_entityFile)
	{
		BinaryIO io;

		m_entityFile = io.open(filePath);

		if (m_entityFile && 0 < m_entityFile->m_size)
		{
			return true;
		}

		LogLocator::getLog().log("Error: Entity file failed to open.");
	}
	else
	{
		LogLocator::getLog().log("Error: Entity file already open. Please close old file before opening new one.");
	}

	return false;
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
	BinaryIO io;

	BB_File* loadFile = io.open(filePath);
	BB_fstream bbstream(loadFile);

	if (bbstream.isOpen())
	{
		int totalCount = 0;
		int currentCount = 0;
		string type = "";

		bbstream >> totalCount;

		if (totalCount <= 0)
		{
			LogLocator::getLog().log("Failed to load entityFile! Total count was 0 or less!");
			return false;
		}

		while (bbstream >> type)
		{
			loadEntityType(type);
			currentCount++;
		}

		if (currentCount == totalCount)
		{
			return true;
		}
		else
		{
			string countString = "Loaded " + std::to_string(currentCount) + " out of " + std::to_string(totalCount) + " entities!";

			LogLocator::getLog().log("Warning: Failed to load all log entities.");
			LogLocator::getLog().log(countString);
		}
	}

	return false;
}

//=============================================================================
// Function: void clearEntityData()
// Description: 
// Clears all of the currently loaded entity data.
//=============================================================================
void EntityManager::clearEntityData()
{
	auto mit = m_entityInfo.begin();

	while (mit != m_entityInfo.end())
	{
		delete mit->second;
		
		mit = m_entityInfo.erase(mit);

		if (mit != m_entityInfo.end())
		{
			mit++;
		}
	}
}

//=============================================================================
// Function: void loadEntityType(const string)
// Description: 
// Loads in the entity type from the currently loaded entity file.
// Parameters: 
// const string type - The entity type to load.
//=============================================================================
void EntityManager::loadEntityType(const string type)
{
	if (m_entityFile != nullptr && type != "")
	{
		auto mit = m_entityInfo.find(type);

		if (mit != m_entityInfo.end())
		{
			mit->second->m_instanceCount++;
		}
		else
		{
			BB_fstream bbstream(m_entityFile);

			EntityInfo* info = new EntityInfo;

			string typeSetting = "<" + type + ">";
			string typeEnd = "</" + type + ">";

			long startPos = bbstream.find(typeSetting.c_str(), typeSetting.length());

			if (-1 < startPos)
			{
				long endPos = bbstream.find(typeEnd.c_str(), typeEnd.length(), startPos);

				if (-1 < endPos)
				{
					bbstream.seek(startPos, BB_fstream::BB_fileBeg);

					bbstream >> info->m_persistant;

					// Load each of the components.
					// TODO: As you add components, add them here.
					string componentName = "<CollisionBox>";

					long componentIndex = bbstream.search(componentName.c_str(), componentName.length(), startPos, endPos);

					if (-1 < componentIndex)
					{
						bbstream.seek(componentIndex + componentName.length(), BB_fstream::BB_fileBeg);

						CollisionBox* box = new CollisionBox();

						if (bbstream >> *box)
						{
							info->m_collisionBox = box;
						}
						else
						{
							delete box;
							LogLocator::getLog().log("Error loading collision box for entity type: " + type + "!");
						}
					}

					// Animator Loading.
					componentName = "<Animator>";

					componentIndex = bbstream.search(componentName.c_str(), componentName.length(), startPos, endPos);

					if (-1 < componentIndex)
					{
						bbstream.seek(componentIndex + componentName.length(), BB_fstream::BB_fileBeg);

						string animationSet = "";

						if (bbstream >> animationSet)
						{
							AnimationSet* set = ResourceManager::getAnimationSet(animationSet);

							if (set)
							{
								Animator* animator = new Animator(set);

								info->m_animator = animator;
							}
							else
							{
								LogLocator::getLog().log("Error: failed to load animation set for entity type: " + type + "!");
							}
						}
						else
						{
							LogLocator::getLog().log("Error: failed to load animator for entity type: " + type + "!");
						}
					}

					// Sprite Component
					// Sprites load in the texture file first.

					componentName = "<Sprite>";

					componentIndex = bbstream.search(componentName.c_str(), componentName.length(), startPos, endPos);

					if (-1 < componentIndex)
					{
						bbstream.seek(componentIndex + componentName.length(), BB_fstream::BB_fileBeg);

						string texturePath = "";

						if (bbstream >> texturePath)
						{
							Texture* texture = ResourceManager::getTexture(texturePath);

							Sprite* sprite = new Sprite();

							if (bbstream >> *sprite)
							{
								sprite->setTexture(texture);
							}
							else
							{
								LogLocator::getLog().log("Error: failed to load sprite!\n");
								delete sprite;
							}
						}
						else
						{
							LogLocator::getLog().log("Error: failed to load texture!\n");
						}
					}
				}

				m_entityInfo.insert(std::make_pair(type, info));
			}
			else
			{
				delete info;
				info = nullptr;
			}
		}
	}
}

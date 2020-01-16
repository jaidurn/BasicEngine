#include "AnimationCache.h"
#include "AnimationSet.h"
#include "LogLocator.h"
#include "BB_fstream.h"
#include "BinaryIO.h"

AnimationCache::AnimationCache()
{
}

AnimationCache::~AnimationCache()
{
	cleanUp();
}

//=============================================================================
// Function: const AnimationSet* getAnimationSet(const string)
// Description:
// Gets the animation set with the specified path.
// Parameters:
// const string setPath - The path to the animation set.
// Output:
// const AnimationSet*
// On success - Returns a pointer to the found animation set.
// On failure - Returns NULL.
//=============================================================================
AnimationSet* AnimationCache::getAnimationSet(const string setPath)
{
	AnimationSet *set = NULL;

	auto mit = m_cache.find(setPath);

	if (mit != m_cache.end())
	{
		set = mit->second;
		set->addInstance();	
	}
	else
	{
		set = loadSet(setPath);
	}

	return set;
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up the cache and frees the memory.
//=============================================================================
void AnimationCache::cleanUp()
{
	auto mit = m_cache.begin();

	while (mit != m_cache.end())
	{
		delete mit->second;
		mit = m_cache.erase(mit);

		if(mit != m_cache.end())
		{
			mit++;
		}
	}
}

//=============================================================================
// Function: AnimationSet* loadSet(const string)
// Description: 
// Loads the animation set from the specified file.
// Parameters: 
// const string setPath - The path to the animation set data.
// Output: 
// AnimationSet*
// On success - Returns the loaded animation set.
// On failure - Returns nullptr.
//=============================================================================
AnimationSet* AnimationCache::loadSet(const string setPath)
{
	if (setPath != "")
	{
		BinaryIO io;

		// TODO: Add in parse when it's parse time.
		BB_File* file = io.open(setPath);

		if (file)
		{
			BB_fstream bbstream(file);

			if (bbstream.isOpen())
			{
				AnimationSet* set = new AnimationSet(setPath);

				bbstream >> *set;

				set->addInstance();
			}
		}
	}

	return nullptr;
}
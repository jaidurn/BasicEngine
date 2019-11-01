#include "AnimationCache.h"
#include "AnimationSet.h"
#include "LogLocator.h"

AnimationCache::AnimationCache()
{
}

AnimationCache::~AnimationCache()
{
	cleanUp();
}

//=============================================================================
// Function: const AnimationSet* getAnimationSet(const string) const
// Description:
// Gets the animation set with the specified name.
// Parameters:
// const string name - The name identifier of the animation set.
// Output:
// const AnimationSet*
// On success - Returns a pointer to the found animation set.
// On failure - Returns NULL.
//=============================================================================
const AnimationSet* AnimationCache::getAnimationSet(const string name) const
{
	AnimationSet *set = NULL;

	auto mit = m_cache.find(name);

	if (mit != m_cache.end())
	{
		set = mit->second;
		set->addInstance();	
	}

	return set;
}

//=============================================================================
// Function: const bool animationSetExists(const string) const
// Description:
// Checks to see if the animation set exists or not.
// Parameters:
// const string name - The name of the animation set to find.
// Output:
// const bool
// Returns true if the animation set exists.
// Returns false if it doesn't.
//=============================================================================
const bool AnimationCache::animationSetExists(const string name) const
{
	bool exists = false;

	if (name != "")
	{
		auto mit = m_cache.find(name);

		if (mit != m_cache.end())
		{
			if (mit->second != NULL)
			{
				exists = true;
			}
		}
	}

	return exists;
}

//=============================================================================
// Function: void addAnimationSet(const string, AnimationSet*)
// Description:
// Adds the provided animation set to the cache under the specified 
// name if the set exists and the name isn't taken.
// Parameters:
// const string name - The identifier of the animation set.
// AnimationSet* set - The set to add.
//=============================================================================
void AnimationCache::addAnimationSet(const string name,
	AnimationSet* set)
{
	if (name != "")
	{
		if (set)
		{
			auto mit = m_cache.find(name);

			if (mit != m_cache.end())
			{
				LogLocator::getLog().log("Failed to add set with name: " + name + "!");
				LogLocator::getLog().log("Name already exists! Deleting set...");

				delete set;
				set = NULL;
			}
			else
			{
				m_cache.insert(std::make_pair(name, set));
			}
		}
		else
		{
			LogLocator::getLog().log("Set for '" + name + "' was NULL.");
		}
	}
	else
	{
		LogLocator::getLog().log("Failed to add the animation set. A blank name is invalid.");
	}
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
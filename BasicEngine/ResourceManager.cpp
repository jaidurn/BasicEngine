#include "ResourceManager.h"
#include "Window.h"
#include "Renderer.h"
#include "TextureCache.h"
#include "AnimationCache.h"
#include "SettingIO.h"

Window* ResourceManager::m_window = NULL;
Renderer* ResourceManager::m_renderer = NULL;
TextureCache* ResourceManager::m_textureCache = NULL;
AnimationCache* ResourceManager::m_animationCache = NULL;
SettingIO* ResourceManager::m_io = NULL;

bool ResourceManager::m_initialized = false;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

//=============================================================================
// Function: Window* getWindow()
// Description:
// Gets the window currently held by the resource manager.
// Output:
// Window*
// On success - Returns the window.
// On failure - Returns NULL.
//=============================================================================
Window* ResourceManager::getWindow()
{
	return m_window;
}

//=============================================================================
// Function: Renderer* getRenderer()
// Description:
// Gets the renderer currently held by the resource manager.
// Output:
// Renderer*
// On success - Returns the renderer.
// On failure - Returns NULL.
//=============================================================================
Renderer* ResourceManager::getRenderer()
{
	return m_renderer;
}

//=============================================================================
// Function: Texture* getTexture(string)
// Description:
// Gets the texture with the specified path.
// Parameters:
// string texturePath - The location of the texture.
// Output:
// Texture*
// On success - Returns the texture.
// On failure - Returns NULL.
//=============================================================================
Texture* ResourceManager::getTexture(string texturePath)
{
	Texture *texture = NULL;
	
	if (m_textureCache)
	{
		texture = m_textureCache->getTexture(texturePath);
	}

	return texture;
}

//=============================================================================
// Function: const AnimationSet* getAnimationSet(const string)
// Description:
// Gets the animation set from the animation cache if it exists.
// Parameters:
// const string name - The unique identifier of the animation 
// to get.
// Output:
// const AnimationSet*
// On success - Returns a pointer to an animation set object.
// On failure - Returns NULL.
//=============================================================================
const AnimationSet* ResourceManager::getAnimationSet(const string name)
{
	if (m_animationCache)
	{
		return m_animationCache->getAnimationSet(name);
	}

	return NULL;
}

//=============================================================================
// Function: void addAnimationSet(const string, AnimationSet*)
// Description:
// Adds an animation set to the animation cache.
// Parameters:
// const string name - The unique identifier of the animation set.
// AnimationSet* set - The set to add.
//=============================================================================
void ResourceManager::addAnimationSet(const string name,
	AnimationSet* set)
{
	if (m_animationCache)
	{
		m_animationCache->addAnimationSet(name, set);
	}
	else
	{

	}
}

//=============================================================================
// Function: void setWindow(Window *window)
// Description:
// Sets the window. If there's already a window, it destroys it.
// Parameters:
// Window *window - The window to use.
//=============================================================================
void ResourceManager::setWindow(Window *window)
{
	if (window)
	{
		if (m_window)
		{
			delete m_window;
		}

		m_window = window;
	}
}

//=============================================================================
// Function: void setRenderer(Renderer *renderer)
// Description:
// Sets the renderer. If there's already a renderer, it destroys it.
// Parameters:
// Renderer *renderer - The renderer to use.
//=============================================================================
void ResourceManager::setRenderer(Renderer *renderer)
{
	if (renderer)
	{
		if (m_renderer)
		{
			delete m_renderer;
		}
		
		m_renderer = renderer;
		
		if (m_textureCache)
		{
			m_textureCache->setRenderer(m_renderer->getRenderer());
		}
	}
}

//=============================================================================
// Function: void setTextureCache(TextureCache*)
// Description:
// Sets the texture cache. If there's already a texture cache, it
// destroys it.
// Parameters:
// TextureCache *textureCache - The texture cache to use.
//=============================================================================
void ResourceManager::setTextureCache(TextureCache *textureCache)
{
	if (textureCache)
	{
		if (m_textureCache)
		{
			delete m_textureCache;
		}

		m_textureCache = textureCache;
	}
}

//=============================================================================
// Function: void loadFromFile(const string)
// Description:
// Loads the resources from a file path if they haven't already
// been loaded.
// Sets m_initialized to true on success.
//=============================================================================
void ResourceManager::loadFromFile(const string loadPath)
{
	// TODO: Fix setting in and out. Remove it to it's own
	// thread.
	if (!m_initialized)
	{
		if (!m_io)
		{
			m_io = new SettingIO("Resources/", ".dat");
		}

		m_io->openFile(loadPath, std::ios::in);

		if (m_io->isFileOpen())
		{
			string title = "";
			int width = 100;
			int height = 100;
			bool fullscreen = false;

			m_io->getSetting("Title", title);
			m_io->getSetting("Width", width);
			m_io->getSetting("Height", height);
			m_io->getSetting("Fullscreen", fullscreen);

			m_window = new Window(title, width, height, fullscreen);

			if (m_window)
			{
				bool vsync = false;

				m_io->getSetting("VsyncEnabled", vsync);

				m_renderer = 
					new Renderer(m_window->getWindow(), vsync);

				if (m_renderer)
				{
					int keyRed = 0;
					int keyGreen = 0;
					int keyBlue = 0;

					m_io->getSetting("ColorKeyRed", keyRed);
					m_io->getSetting("ColorKeyGreen", keyGreen);
					m_io->getSetting("ColorKeyBlue", keyBlue);
					
					m_textureCache = 
						new TextureCache(m_renderer->getRenderer(), 
							keyRed,
							keyGreen,
							keyBlue);

					if (m_textureCache)
					{
						m_animationCache = new AnimationCache();

						m_initialized = true;
					}
				}
			}
		}
	}

	if (!m_initialized)
	{
		cleanUp();
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// This function is meant to be called when everything needs
// to be deleted. This will free all the memory used and
// set all the static members to NULL.
//=============================================================================
void ResourceManager::cleanUp()
{
	if (m_io)
	{
		delete m_io;
		m_io = NULL;
	}

	if (m_animationCache)
	{
		delete m_animationCache;
		m_animationCache = NULL;
	}

	if (m_textureCache)
	{
		delete m_textureCache;
		m_textureCache = NULL;
	}	

	if (m_renderer)
	{
		delete m_renderer;
		m_renderer = NULL;
	}	

	if (m_window)
	{
		delete m_window;
		m_window = NULL;
	}
	
	m_initialized = false;
}

//=============================================================================
// Function: void saveSettings()
// Description:
// Saves the current settings to the settings file.
//=============================================================================
void ResourceManager::saveSettings()
{
	if (m_initialized)
	{
		if (m_io)
		{
			m_io->saveSetting("Title", m_window->getTitle());
			m_io->saveSetting("Width", m_window->getWidth());
			m_io->saveSetting("Height", m_window->getHeight());

			bool vsync = false;

			if (m_renderer->getRenderFlags() == SDL_RENDERER_PRESENTVSYNC)
			{
				vsync = true;
			}
			
			m_io->saveSetting("VsyncEnabled", vsync);
		}
	}
}

//=============================================================================
// Function: bpp; initialized()
// Description:
// Gets the initialization state of the manager.
// Output:
// bool
// Returns true if the resource manager is initialized.
// Returns false if it isn't.
//=============================================================================
bool ResourceManager::initialized()
{
	return m_initialized;
}
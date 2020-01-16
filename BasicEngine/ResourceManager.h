#pragma once
//==========================================================================================
// File Name: ResourceManager.h
// Author: Brian Blackmon
// Date Created: 10/19/2019
// Purpose: 
// It's a locator for locating the resource items.
//==========================================================================================
#include <string>

class Window;
class Renderer;
class TextureCache;
class Texture;
class SettingIO;
class AnimationCache;
class AnimationSet;

typedef std::string string;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static Window* getWindow();
	static Renderer* getRenderer();

	static Texture* getTexture(const string texturePath);
	static AnimationSet* getAnimationSet(const string name);

	static void setWindow(Window *window);
	static void setRenderer(Renderer *renderer);
	static void setTextureCache(TextureCache *textureCache);

	static void loadFromFile(const string loadPath);
	static void cleanUp();
	static void saveSettings();

	static bool initialized();

private:
	static Window *m_window;
	static Renderer *m_renderer;
	static TextureCache *m_textureCache;
	static AnimationCache *m_animationCache;
	static SettingIO *m_io;

	static bool m_initialized;


};


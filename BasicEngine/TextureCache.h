#pragma once
//==========================================================================================
// File Name: 
// Author: 
// Date Created: 
// Purpose: 
// Allows for creation, storing, and accessing textures.
// This keeps the textures from being created multiple times.
//==========================================================================================
#include <map>
#include <string>
#include <SDL.h>

class Texture;

typedef std::string string;

class TextureCache
{
public:
	TextureCache(SDL_Renderer *renderer,
		const Uint8 redKey,
		const Uint8 greenKey,
		const Uint8 blueKey);
	~TextureCache();

	Texture* getTexture(string texturePath);

	void setRenderer(SDL_Renderer *renderer);

private:
	std::map<string, Texture*> m_cache;

	SDL_Renderer *m_renderer;

	Uint8 m_redKey;
	Uint8 m_greenKey;
	Uint8 m_blueKey;

	void cleanUp();
};


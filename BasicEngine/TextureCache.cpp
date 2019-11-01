#include "TextureCache.h"
#include <SDL_image.h>
#include "Texture.h"
#include "LogLocator.h"

TextureCache::TextureCache(SDL_Renderer *renderer,
	const Uint8 redKey,
	const Uint8 greenKey,
	const Uint8 blueKey)
	:m_renderer(renderer),
	m_redKey(redKey),
	m_greenKey(greenKey),
	m_blueKey(blueKey)
{
}

TextureCache::~TextureCache()
{
	cleanUp();
}

//=============================================================================
// Function: Texture* getTexture(string)
// Description:
// Finds the texture with the specified texture path. If it 
// doesn't exist, it creates it.
// Parameters:
// string texturePath - The texture path of the texture.
// Output:
// Texture*
// On success - Returns the texture that was found/created.
// On failure - Returns NULL.
//=============================================================================
Texture* TextureCache::getTexture(string texturePath)
{
	Texture *texture = NULL;

	if (texturePath != "" && m_renderer)
	{
		auto cacheIt = m_cache.find(texturePath);

		if (cacheIt != m_cache.end())
		{
			texture = cacheIt->second;
			texture->addInstance();
		}
		else
		{
			SDL_Surface *surface = IMG_Load(texturePath.c_str());

			if (surface)
			{
				if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, m_redKey, m_greenKey, m_blueKey)) != 0)
				{
					LogLocator::getLog().log("Failed to set color key.");
				}
				else
				{
					SDL_Texture *temp = SDL_CreateTextureFromSurface(m_renderer, surface);

					if (temp)
					{
						Rectangle rect(Vector2D((float)(surface->w / 2), (float)(surface->h / 2)),
							surface->w,
							surface->h);

						texture = new Texture(temp, rect);

						m_cache.insert(std::make_pair(texturePath, texture));

						texture->addInstance();
					}
					else
					{
						LogLocator::getLog().log("Failed to create texture!");
					}
				}

				SDL_FreeSurface(surface);
			}
		}
	}
	else
	{
		LogLocator::getLog().log("Failed to create texture with path: " + texturePath);
	}

	return texture;
}

//=============================================================================
// Function: void setRenderer(SDL_Renderer*)
// Description:
// Switches the renderer to use for creating textures.
// Parameters:
// SDL_Renderer *renderer - The renderer to change to.
//=============================================================================
void TextureCache::setRenderer(SDL_Renderer *renderer)
{
	if (renderer)
	{
		m_renderer = renderer;
	}
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up all of the textures.
//=============================================================================
void TextureCache::cleanUp()
{
	auto cacheMit = m_cache.begin();

	while (cacheMit != m_cache.end())
	{
		delete cacheMit->second;
		
		cacheMit = m_cache.erase(cacheMit);

		if (cacheMit != m_cache.end())
		{
			cacheMit++;
		}
	}

	m_renderer = NULL;
}
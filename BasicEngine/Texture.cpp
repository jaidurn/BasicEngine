#include "Texture.h"
#include <SDL.h>

Texture::Texture(SDL_Texture *texture,
	const Rectangle& bounds,
	string path)
	:m_texture(texture),
	m_rect(bounds),
	m_path(path),
	m_instanceCount(0)
{

}

Texture::~Texture()
{
	if (m_texture)
	{
		SDL_DestroyTexture(m_texture);
	}
}

//=============================================================================
// Function: const Rectangle& getRect() const
// Description:
// Gets the rectangle containing the texture size and location. 
// Output:
// const Rectangle&
// Returns a reference to the bound rectangle.
//=============================================================================
const Rectangle& Texture::getRect() const
{
	return m_rect;
}

//=============================================================================
// Function: const int getWidth() const
// Description:
// Gets the width of the texture.
// Output:
// const int
// Returns the width of the texture.
//=============================================================================
const int Texture::getWidth() const
{
	return m_rect.getWidth();
}

//=============================================================================
// Function: const int getHeight() const
// Description:
// Gets the height of the texture.
// Output:
// const int
// Returns the height of the texture.
//=============================================================================
const int Texture::getHeight() const
{
	return m_rect.getHeight();
}

//=============================================================================
// Function: const int getInstanceCount() const
// Description:
// Gets the instance count of the texture. An instance is
// anything that uses the texture.
// Output:
// const int
// Returns the count of current instances using a texture.
//=============================================================================
const int Texture::getInstanceCount() const
{
	return m_instanceCount;
}

//=============================================================================
// Function: SDL_Texture* getTexture() const
// Description:
// Gets the SDL_Texture struct.
// Output:
// SDL_Texture*
// Returns a pointer to the SDL_Texture object.
//=============================================================================
SDL_Texture* Texture::getTexture() const
{
	return m_texture;	
}

//=============================================================================
// Function: string getPath() const
// Description: 
// Gets the path of the current texture.
// Output: 
// string
// Returns the filepath to the current texture object.
//=============================================================================
string Texture::getPath() const
{
	return m_path;
}

//=============================================================================
// Function: void addInstance()
// Description:
// Increases the instance counter for the texture.
//=============================================================================
void Texture::addInstance()
{
	m_instanceCount++;
}

//=============================================================================
// Function: void removeInstance()
// Description:
// Decreases the instance counter for the texture.
//=============================================================================
void Texture::removeInstance()
{
	m_instanceCount--;
}
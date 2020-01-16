#pragma once
//==========================================================================================
// File Name: Texture.h
// Author: Brian Blackmon
// Date Created: 10/19/2019
// Purpose: 
// Wraps the SDL_Texture struct and adds addtional functionality.
//==========================================================================================
#include "Rectangle.h"

struct SDL_Texture;

class Texture
{
public:
	Texture(SDL_Texture *texture,
			const Rectangle& bounds, 
			string path);
	~Texture();

	const Rectangle& getRect() const;
	const int getWidth() const;
	const int getHeight() const;
	const int getInstanceCount() const;

	SDL_Texture* getTexture() const;
	string getPath() const;

	void addInstance();
	void removeInstance();

private:
	SDL_Texture *m_texture;
	Rectangle m_rect;
	string m_path;

	int m_instanceCount;
};


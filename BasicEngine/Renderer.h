#pragma once
//==========================================================================================
// File Name: Renderer.h
// Author: Brian Blackmon
// Date Created: 10/16/2019
// Purpose: 
// Wraps the SDL_Renderer struct and adds functionality.
//==========================================================================================
#include <SDL.h>
#include "Rectangle.h"
#include "Line.h"

class Texture;

class Renderer
{
public:
	Renderer(SDL_Window *window, 
		const bool vsyncEnabled);
	~Renderer();

	SDL_Renderer* getRenderer() const;
	const SDL_BlendMode getBlendMode() const;
	const Uint32 getRenderFlags() const;

	void setBlendMode(const SDL_BlendMode mode);
	void setClearColor(const SDL_Color color);
	void setDrawColor(const SDL_Color color);

	void clear();
	void update();

	void drawTexture(const Texture& texture,
		const Rectangle& dest);

	void drawTexture(const Texture& texture,
		const Rectangle& clip,
		const Rectangle& dest);

	void drawTexture(const Texture& texture,
		const Rectangle& clip,
		const Rectangle& dest,
		const Vector2D& anchor);

	// Drawing tools.
	void drawRect(const Rectangle& rect, 
		const SDL_Color color, 
		bool filled = false);

	void drawLine(const Line& line,
		const SDL_Color color);

	void drawPoint(const Vector2D& point,
		const SDL_Color color);

private:
	SDL_Renderer *m_renderer;
	SDL_Color m_clearColor;
};
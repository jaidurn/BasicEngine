#pragma once
//==========================================================================================
// File Name: Window.h
// Author: Brian Blackmon
// Date Created: 10/16/2019
// Purpose: 
// Wraps the SDL_Window struct and provides functionality.
//==========================================================================================
#include <SDL.h>
#include <string>

typedef std::string string;

class Window
{
public:
	Window(const string title, 
		const int width, 
		const int height, 
		const bool fullscreen);
	~Window();

	SDL_Window* getWindow() const;
	const int getWidth() const;
	const int getHeight() const;
	const string getTitle() const;
	const bool getFullscreen() const;
	const SDL_PixelFormat* getPixelFormat() const;

	void setWidth(const int width);
	void setHeight(const int height);
	void setTitle(const string title);
	void setFullscreen(const bool fullscreen);

private:
	SDL_Window *m_window;
	SDL_PixelFormat *m_format;
};


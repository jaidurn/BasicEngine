#include "Window.h"
#include <iostream>
#include "LogLocator.h"

Window::Window(const string title,
	const int width,
	const int height,
	const bool fullscreen)
	:m_window(NULL),
	m_format(NULL)
{
	Uint32 windowFlag = SDL_WINDOW_SHOWN;

	if (fullscreen)
	{
		windowFlag |= SDL_WINDOW_FULLSCREEN;
	}

	m_window =
		SDL_CreateWindow(title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			windowFlag);

	if (!m_window)
	{
		string errorMessage = "Failed to create window. SDL Error: ";
		errorMessage += SDL_GetError();

		LogLocator::getLog().log(errorMessage);
	}
	else
	{
		Uint32 pixelFormat = SDL_GetWindowPixelFormat(m_window);

		if (pixelFormat == SDL_PIXELFORMAT_UNKNOWN)
		{
			string pixelError = "SDL Error: ";
			pixelError += SDL_GetError();

			LogLocator::getLog().log("Failed to load the window pixel format.");
			LogLocator::getLog().log(pixelError);
		}
		else
		{
			m_format = SDL_AllocFormat(pixelFormat);

			LogLocator::getLog().log("Window created!");
		}
	}
}

Window::~Window()
{
	if (m_format)
	{
		SDL_FreeFormat(m_format);
		m_format = NULL;
	}

	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		LogLocator::getLog().log("Window destroyed!");
	}
}

//=============================================================================
// Function: SDL_Window* getWindow() const
// Description:
// Gets the SDL_Window.
// Output:
// SDL_Window*
// On success - Returns a pointer to the SDL_Window struct.
// On failure - Returns NULL.
//=============================================================================
SDL_Window* Window::getWindow() const
{
	return m_window;
}

//=============================================================================
// Function: const int getWidth() const 
// Description:
// Gets the width of the current SDL_Window.
// Output:
// const int
// On success - Returns the width of the window.
// On failure - Returns -1.
//=============================================================================
const int Window::getWidth() const
{
	int width = -1;

	if (m_window)
	{
		SDL_GetWindowSize(m_window, &width, NULL);
	}

	return width;
}

//=============================================================================
// Function: const int getHeight() const
// Description:
// Gets the height of the current SDL_Window.
// Output:
// const int
// On success - Returns the height of the window.
// On failure - Returns -1.
//=============================================================================
const int Window::getHeight() const
{
	int height = -1;

	if (m_window)
	{
		SDL_GetWindowSize(m_window, NULL, &height);
	}

	return height;
}

//=============================================================================
// Function: const string getTitle() const
// Description:
// Gets the title of the window.
// Output:
// const string
// On success - Returns a string containing the title.
// On failure - Returns a string containing "".
//=============================================================================
const string Window::getTitle() const
{
	string title = "";

	if (m_window)
	{
		title = SDL_GetWindowTitle(m_window);
	}

	return title;
}

//=============================================================================
// Function: const bool getFullscreen() const
// Description:
// Gets the fullscreen state of the window.
// Output:
// const bool
// Returns true if the window is fullscreen.
// Returns false if the window is windowed.
//=============================================================================
const bool Window::getFullscreen() const
{
	bool fullscreen = false;

	if (m_window)
	{
		Uint32 windowFlags = SDL_GetWindowFlags(m_window);

		if (windowFlags & SDL_WINDOW_FULLSCREEN)
		{
			fullscreen = true;
		}
	}

	return fullscreen;
}

//=============================================================================
// Function: const SDL_PixelFormat* getPixelFormat() const
// Description:
// Gets the pixel format of the window.
// Output:
// const SDL_PixelFormat
// On success returns the pixel format.
// On failure returns NULL.
//=============================================================================
const SDL_PixelFormat* Window::getPixelFormat() const
{
	return m_format;
}

//=============================================================================
// Function: void setWidth(const int)
// Description:
// Sets the width of the window.
// Parameters:
// const int width - The new window width.
//=============================================================================
void Window::setWidth(const int width)
{
	if (0 < width)
	{
		if (m_window)
		{
			int height = -1;

			SDL_GetWindowSize(m_window, NULL, &height);

			if (height != -1)
			{
				SDL_SetWindowSize(m_window, width, height);
			}
			else
			{
				LogLocator::getLog().log("Failed to set window width!");
			}
		}
	}
}

//=============================================================================
// Function: void setHeight(const int)
// Description:
// Sets the height of the window.
// Parameters:
// const int height - The new height of the window.
//=============================================================================
void Window::setHeight(const int height)
{
	if (0 < height)
	{
		if (m_window)
		{
			int width = -1;

			SDL_GetWindowSize(m_window, &width, NULL);

			if (width != -1)
			{
				SDL_SetWindowSize(m_window, width, height);

			}
			else
			{
				LogLocator::getLog().log("Failed to set the window height!");
			}
		}
	}
}

//=============================================================================
// Function: void setTitle(const string)
// Description:
// Sets the window title.
// Parameters:
// const string title - The new window title.
//=============================================================================
void Window::setTitle(const string title)
{
	if (title != "" && m_window)
	{
		SDL_SetWindowTitle(m_window, title.c_str());
	}
}

//=============================================================================
// Function: void setFullscreen(const bool)
// Description:
// Sets the window's fullscreen state.
// Parameters:
// const bool fullscreen - The new fullscreen state.
//=============================================================================
void Window::setFullscreen(const bool fullscreen)
{
	if (m_window)
	{
		if (fullscreen)
		{
			if (SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN) != 0)
			{
				string sdlError = "Failed to fullscreen window! SDL Error: ";
				sdlError += SDL_GetError();

				LogLocator::getLog().log(sdlError);
			}
		}
		else
		{
			if (SDL_SetWindowFullscreen(m_window, 0) != 0)
			{
				string sdlError = "SDL Error: ";
				sdlError += SDL_GetError();

				LogLocator::getLog().log("Failed to set the window to windowed mode.");
				LogLocator::getLog().log(sdlError);
			}
		}
	}
}
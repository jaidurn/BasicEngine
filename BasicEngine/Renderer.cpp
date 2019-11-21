#include "Renderer.h"
#include "LogLocator.h"
#include "Texture.h"

Renderer::Renderer(SDL_Window *window,
	const bool vsyncEnabled)
	:m_renderer(NULL)
{
	m_clearColor.r = 0;
	m_clearColor.g = 0;
	m_clearColor.b = 0;
	m_clearColor.a = 255;

	if (window)
	{
		Uint32 flags = SDL_RENDERER_ACCELERATED;

		if (vsyncEnabled)
		{
			flags = SDL_RENDERER_PRESENTVSYNC;
		}

		m_renderer = SDL_CreateRenderer(window, -1, flags);
		
		if (m_renderer)
		{
			LogLocator::getLog().log("Created renderer!");
		}
		else
		{
			string sdlError = "SDL Error: ";
			sdlError += SDL_GetError();

			LogLocator::getLog().log("Failed to create renderer!");
			LogLocator::getLog().log(sdlError);
		}
	}
	else
	{
		LogLocator::getLog().log("Failed to create renderer! No window provided!");
	}
}


Renderer::~Renderer()
{
	if (m_renderer)
	{
		SDL_DestroyRenderer(m_renderer);

		LogLocator::getLog().log("Destroyed renderer!");
	}
}

//=============================================================================
// Function: SDL_Renderer* getRenderer() const
// Description:
// Gets the current SDL Renderer.
// Output:
// SDL_Renderer*
// On success - Returns a pointer to the SDL renderer.
// On failure - Returns NULL.
//=============================================================================
SDL_Renderer* Renderer::getRenderer() const
{
	return m_renderer;
}

//=============================================================================
// Function: const SDL_BlendMode getBlendMode() const
// Description:
// Gets the current Renderer blend mode.
// Output:
// const SDL_BlendMode
// Returns the renderer blend mode.
//=============================================================================
const SDL_BlendMode Renderer::getBlendMode() const
{
	SDL_BlendMode blend = SDL_BLENDMODE_BLEND;

	if (m_renderer)
	{
		SDL_GetRenderDrawBlendMode(m_renderer, &blend);
	}

	return blend;
}

//=============================================================================
// Function: const Uint32 getRenderFlags() const
// Description:
// Gets the current flags suppored by the renderer.
// Output:
// const Uint32
// On success - Returns the Renderer flags.
// On failure - Returns -1.
//=============================================================================
const Uint32 Renderer::getRenderFlags() const
{
	Uint32 flags = -1;

	if (m_renderer)
	{
		SDL_RendererInfo *info = NULL;

		if (SDL_GetRendererInfo(m_renderer, info) != 0)
		{
			string infoError = "SDL Error: ";
			infoError += SDL_GetError();

			LogLocator::getLog().log("Failed to get renderer info.");
			LogLocator::getLog().log(infoError);
		}
		else
		{
			flags = info->flags;

			delete info;
		}
	}

	return flags;
}

//=============================================================================
// Function: void setBlendMode(const SDL_BlendMode)
// Description:
// Sets the Renderer's blend mode.
// Parameters:
// const SDL_BlendMode blend - The new blend mode to use.
//=============================================================================
void Renderer::setBlendMode(const SDL_BlendMode blend)
{
	if (m_renderer)
	{
		if (SDL_SetRenderDrawBlendMode(m_renderer, blend) != 0)
		{
			string sdlError = "SDL Error: ";
			sdlError += SDL_GetError();

			LogLocator::getLog().log(sdlError);
		}
	}
}

//=============================================================================
// Function: void setClearColor(const SDL_Color)
// Description:
// Sets the color the renderer clears to.
// Parameters:
// const SDL_Color color - The new clear color.
//=============================================================================
void Renderer::setClearColor(const SDL_Color color)
{
	m_clearColor.r = color.r;
	m_clearColor.g = color.g;
	m_clearColor.b = color.b;
	m_clearColor.a = color.a;
}

//=============================================================================
// Function: void setDrawColor(const SDL_Color color)
// Description:
// Sets the renderer draw color for the current frame.
// Parameters:
// const SDL_Color color - The new draw color to use.
//=============================================================================
void Renderer::setDrawColor(const SDL_Color color)
{
	if (m_renderer)
	{
		SDL_SetRenderDrawColor(m_renderer,
			color.r,
			color.g,
			color.b,
			color.a);
	}
}

//=============================================================================
// Function: void clear()
// Description:
// Clears the renderer to the default color.
//=============================================================================
void Renderer::clear()
{
	if (m_renderer)
	{
		setDrawColor(m_clearColor);

		SDL_RenderClear(m_renderer);
	}
}

//=============================================================================
// Function: void update()
// Description:
// Updates the render to show what's been drawn.
//=============================================================================
void Renderer::update()
{
	if (m_renderer)
	{
		SDL_RenderPresent(m_renderer);
	}
}

//=============================================================================
// Function: void drawTexture(const Texture*, const Rectangle&)
// Description:
// Draws the texture at the specified destination.
// Parameters:
// const Texture *texture - The texture to render.
// const Rectangle& dest - The render destination.
//=============================================================================
void Renderer::drawTexture(const Texture& texture,
	const Rectangle& dest)
{
	if (m_renderer)
	{
		int topLeftX =
			(int)(dest.getCenter().m_x - (dest.getWidth() / 2));
		int topLeftY =
			(int)(dest.getCenter().m_y - (dest.getHeight() / 2));

		SDL_Rect sdlDest{ topLeftX,
		topLeftY,
		dest.getWidth(),
		dest.getHeight() };

		if (dest.getRotation() != 0.0f)
		{
			double angle = 
				(double)(dest.getRotation() / 100.0f * 360.0f);

			if (SDL_RenderCopyEx(m_renderer,
				texture.getTexture(),
				NULL,
				&sdlDest,
				angle,
				NULL,
				SDL_FLIP_NONE) != 0)
			{
				string renderError = "SDL Error: ";
				renderError += SDL_GetError();

				LogLocator::getLog().log("Failed to render texture.");
				LogLocator::getLog().log(renderError);
			}
		}
		else
		{
			if(SDL_RenderCopy(m_renderer,
				texture.getTexture(),
				NULL,
				&sdlDest) != 0)
			{
				string renderError = "SDL Error: ";
				renderError += SDL_GetError();

				LogLocator::getLog().log("Failed to render texture.");
				LogLocator::getLog().log(renderError);
			}
		}
	}
}

//=============================================================================
// Function: void drawTexture(const Texture*, 
// const Rectangle&,
// const Rectangle&)
// Description:
// Draws the texture at the specified destination.
// Parameters:
// const Texture *texture - The texture to render.
// const Rectangle& clip - The clip to use.
// const Rectangle& dest - The render destination.
//=============================================================================
void Renderer::drawTexture(const Texture& texture,
	const Rectangle& clip,
	const Rectangle& dest)
{
	if (m_renderer)
	{
		SDL_Rect sdlClip{ (int)clip.getTopLeft().m_x,
		(int)clip.getTopLeft().m_y,
		clip.getWidth(),
		clip.getHeight() };

		int topLeftX =
			(int)(dest.getCenter().m_x - (dest.getWidth() / 2));
		int topLeftY =
			(int)(dest.getCenter().m_y - (dest.getHeight() / 2));

		SDL_Rect sdlDest{ topLeftX,
		topLeftY,
		dest.getWidth(),
		dest.getHeight() };

		// If the destination is rotated, use the advanced call.
		if (dest.getRotation() != 0.0f)
		{
			double angle = 
				(double)(dest.getRotation() / 100.0f * 360.0f);

			if (SDL_RenderCopyEx(m_renderer,
				texture.getTexture(),
				&sdlClip,
				&sdlDest,
				angle,
				NULL,
				SDL_FLIP_NONE) != 0)
			{
				string renderError = "SDL Error: ";
				renderError += SDL_GetError();

				LogLocator::getLog().log("Failed to render texture.");
				LogLocator::getLog().log(renderError);
			}
		}
		else
		{

			if (SDL_RenderCopy(m_renderer,
				texture.getTexture(),
				&sdlClip,
				&sdlDest) != 0)
			{
				string renderError = "SDL Error: ";
				renderError += SDL_GetError();

				LogLocator::getLog().log("Failed to render texture.");
				LogLocator::getLog().log(renderError);
			}
		}
	}
}

//=============================================================================
// Function: void drawTexture(const Texture*, 
// const Rectangle&,
// const Rectangle&,
// const Vector2D&)
// Description:
// Draws the texture at the specified destination.
// Parameters:
// const Texture *texture - The texture to render.
// const Rectangle& clip - The clip to use.
// const Rectangle& dest - The render destination.
// const Vector2D& anchor - The rotation anchor point.
//=============================================================================
void Renderer::drawTexture(const Texture& texture,
	const Rectangle& clip,
	const Rectangle& dest,
	const Vector2D& anchor)
{
	if (m_renderer)
	{
		SDL_Rect sdlClip{ (int)clip.getTopLeft().m_x,
		(int)clip.getTopLeft().m_y,
		clip.getWidth(),
		clip.getHeight() };

		int topLeftX =
			(int)(dest.getCenter().m_x - (dest.getWidth() / 2));
		int topLeftY =
			(int)(dest.getCenter().m_y - (dest.getHeight() / 2));

		SDL_Rect sdlDest{ topLeftX,
		topLeftY,
		dest.getWidth(),
		dest.getHeight() };

		// If the destination is rotated, use the advanced call.
		if (dest.getRotation() != 0.0f)
		{
			double angle =
				(double)(dest.getRotation() / 100.0f * 360.0f);

			SDL_Point sdlAnchor{ (int)anchor.m_x, (int)anchor.m_y };

			if (SDL_RenderCopyEx(m_renderer,
				texture.getTexture(),
				&sdlClip,
				&sdlDest,
				angle,
				&sdlAnchor,
				SDL_FLIP_NONE) != 0)
			{
				string renderError = "SDL Error: ";
				renderError += SDL_GetError();

				LogLocator::getLog().log("Failed to render texture.");
				LogLocator::getLog().log(renderError);
			}
		}
		else
		{

			if (SDL_RenderCopy(m_renderer,
				texture.getTexture(),
				&sdlClip,
				&sdlDest) != 0)
			{
				string renderError = "SDL Error: ";
				renderError += SDL_GetError();

				LogLocator::getLog().log("Failed to render texture.");
				LogLocator::getLog().log(renderError);
			}
		}
	}
}

//=============================================================================
// Function: void drawRect(const Rectangle&, const SDL_Color, bool)
// Description:
// Draws a rectangle with the provided color. If filled is true, it fills
// the entirety of the rectangle with color.
// Parameters:
// const Rectangle& rect - The rectangle to draw.
// const SDL_Color color - The color to use.
// bool filled - Whether to fill in the rect or not.
//=============================================================================
void Renderer::drawRect(const Rectangle& rect,
	const SDL_Color color,
	bool filled)
{
	if (m_renderer)
	{
		setDrawColor(color);

		SDL_Rect dest{ (int)roundf(rect.getTopLeft().m_x),
					   (int)roundf(rect.getTopLeft().m_y),
					   rect.getWidth(),
					   rect.getHeight() };

		if (filled)
		{
			SDL_RenderFillRect(m_renderer, &dest);
		}
		else
		{
			if (rect.getRotation() == 0.0f)
			{
				SDL_RenderDrawRect(m_renderer, &dest);
			}
			else
			{
				Rectangle temp = rect;
				Vector2D roundCenter = temp.getCenter();

				roundCenter.m_x = roundf(roundCenter.m_x);
				roundCenter.m_y = roundf(roundCenter.m_y);
				
				temp.setCenter(roundCenter);

				Line top(temp.getTopLeft(), temp.getTopRight());
				Line bottom(temp.getBottomLeft(), temp.getBottomRight());
				Line right(temp.getTopRight(), temp.getBottomRight());
				Line left(temp.getTopLeft(), temp.getBottomLeft());

				roundLine(top);
				roundLine(bottom);
				roundLine(right);
				roundLine(left);

				drawLine(top, color);
				drawLine(bottom, color);
				drawLine(right, color);
				drawLine(left, color);
			}
		}
	}
}

//=============================================================================
// Function: void drawLine(const Line&, const SDL_Color)
// Description:
// Draws a line in the specified color.
// Parameters:
// const Line& line - The line to draw.
// const SDL_Color color - The color to use.
//=============================================================================
void Renderer::drawLine(const Line& line, const SDL_Color color)
{
	if (m_renderer)
	{
		setDrawColor(color);

		if (SDL_RenderDrawLine(m_renderer,
			(int)roundf(line.m_start.m_x),
			(int)roundf(line.m_start.m_y),
			(int)roundf(line.m_end.m_x),
			(int)roundf(line.m_end.m_y)) != 0)
		{
			LogLocator::getLog().log("Failed to draw line!");

			string sdlError = "SDL Error: ";
			sdlError += SDL_GetError();

			LogLocator::getLog().log(sdlError);
		}
	}
}

//=============================================================================
// Function: void drawPoint(const Vector2D&, const SDL_Color)
// Description:
// Colors the pixel at the point the specified color.
// Parameters:
// const Vector2D& point - The location to draw.
// const SDL_Color color - The color to use.
//=============================================================================
void Renderer::drawPoint(const Vector2D& point,
	const SDL_Color color)
{
	if (m_renderer)
	{
		setDrawColor(color);

		if (SDL_RenderDrawPoint(m_renderer,
			(int)point.m_x,
			(int)point.m_y) != 0)
		{
			LogLocator::getLog().log("Failed to draw point.");

			string sdlError = "SDL Error: ";
			sdlError += SDL_GetError();

			LogLocator::getLog().log(sdlError);
		}
	}
}
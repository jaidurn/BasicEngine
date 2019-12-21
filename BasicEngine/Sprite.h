#pragma once
//==========================================================================================
// File Name: Sprite.h
// Author: Brian Blackmon
// Date Created: 10/23/2019
// Purpose: 
// Holds information for the sprites.
//==========================================================================================
#include "Rectangle.h"

class Texture;

class Sprite
{
public:
	Sprite(Texture* texture,
		const Rectangle& size,
		const Rectangle& clip,
		const Vector2D& anchor,
		const int layer = 0);
	~Sprite();

	Sprite& operator=(const Sprite& sprite);

	const Rectangle& getSize() const;
	const Rectangle& getClip() const;
	
	const float getRotation() const;
	
	const Vector2D& getPosition() const;
	const Vector2D& getAnchor() const;
	
	Texture* getTexture() const;

	const int getLayer() const;

	void setWidth(const int width);
	void setHeight(const int height);
	
	void setRotation(const float amount);

	void setPosition(const Vector2D& position);
	void setAnchor(const Vector2D& anchor);
	
	void setTexture(Texture* texture);
	
	void setClip(const Rectangle& clip);

	void setLayer(const int layer);

private:
	Texture* m_texture;
	
	Rectangle m_size;
	Rectangle m_clip;

	Vector2D m_anchor;

	int m_layer;
};


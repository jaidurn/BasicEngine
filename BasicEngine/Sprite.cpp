#include "Sprite.h"
#include "Texture.h"

#define NULL 0

Sprite::Sprite(Texture *texture,
	const Rectangle& size,
	const Rectangle& clip,
	const Vector2D& anchor,
	const int layer)
	:m_texture(texture),
	m_size(size),
	m_clip(clip),
	m_anchor(anchor),
	m_layer(layer)
{

}

Sprite::~Sprite()
{
	m_texture->removeInstance();
	m_texture = NULL;
}

//=============================================================================
// Function: const Rectangle& getSize() const
// Description:
// Gets the size rectangle.
// Output:
// const Rectangle&
// Returns a reference to the size rectangle.
//=============================================================================
const Rectangle& Sprite::getSize() const
{
	return m_size;
}

//=============================================================================
// Function: const Rectangle& getClip() const
// Description:
// Gets the current clip rectangle.
// Output:
// const Rectangle&
// Returns a reference to the current clip rectangle.
//=============================================================================
const Rectangle& Sprite::getClip() const
{
	return m_clip;
}

//=============================================================================
// Function: const float getRotation() const
// Description:
// Gets the rotation of the sprite.
// Output:
// const float
// Returns the current rotation of the sprite.
//=============================================================================
const float Sprite::getRotation() const
{
	return m_size.getRotation();
}

//=============================================================================
// Function: const Vector2D& getPosition() const
// Description:
// Gets the center position of the size rectangle.
// Output:
// const Vector2D&
// Returns the center position.
//=============================================================================
const Vector2D& Sprite::getPosition() const
{
	return m_size.getCenter();
}

//=============================================================================
// Function: const Vector2D& getAnchor() const
// Description:
// Gets the rotation anchor point.
// Output:
// const Vector2D&
// Returns the position of the anchor.
//=============================================================================
const Vector2D& Sprite::getAnchor() const
{
	return m_anchor;
}

//=============================================================================
// Function: Texture* getTexture() const
// Description:
// Gets the current texture.
// Output:
// Texture*
// Returns a pointer to the current texture.
// Returns NULL if there isn't one.
//=============================================================================
Texture* Sprite::getTexture() const
{
	return m_texture;
}

//=============================================================================
// Function: const int getLayer() const
// Description:
// Gets the current sprite layer.
// Output:
// const int
// Returns the layer.
//=============================================================================
const int Sprite::getLayer() const
{
	return m_layer;
}

//=============================================================================
// Function: void setWidth(const int)
// Description:
// Sets the width of the sprite.
// Parameters:
// const int width - The new sprite width.
//=============================================================================
void Sprite::setWidth(const int width)
{
	m_size.setWidth(width);
}

//=============================================================================
// Function: void setHeight(const int)
// Description:
// Sets the height of the sprite.
// Parameters:
// const int height - The new sprite height.
//=============================================================================
void Sprite::setHeight(const int height)
{
	m_size.setHeight(height);
}

//=============================================================================
// Function: void setRotation(const float)
// Description:
// Sets the rotation amount of the sprite.
// Parameters:
// const float amount - The new sprite rotation amount.
//=============================================================================
void Sprite::setRotation(const float amount)
{
	m_size.setRotation(amount);
}

//=============================================================================
// Function: void setPosition(const Vector2D&)
// Description:
// Sets the new center position of the sprite.
// Parameters:
// const Vector2D& position - The new sprite position.
//=============================================================================
void Sprite::setPosition(const Vector2D& position)
{
	m_size.setCenter(position.m_x, position.m_y);
}

//=============================================================================
// Function: void setAnchor(const Vector2D&)
// Description:
// Sets the rotation anchor point of the sprite.
// Parameters:
// const Vector2D& anchor - The new anchor point.
//=============================================================================
void Sprite::setAnchor(const Vector2D& anchor)
{
	m_anchor = anchor;
}

//=============================================================================
// Function: void setTexture(Texture*)
// Description:
// Sets the texture for the sprite.
// Parameters:
// Texture* texture - The new texture to use.
//=============================================================================
void Sprite::setTexture(Texture *texture)
{
	if (m_texture)
	{
		m_texture->removeInstance();
	}

	m_texture = texture;
}

//=============================================================================
// Function: void setClip(const Rectangle&)
// Description:
// Sets the new clip to use for the sprite.
// Parameters:
// const Rectangle& clip - The new clip to use.
//=============================================================================
void Sprite::setClip(const Rectangle& clip)
{
	m_clip = clip;
}

//=============================================================================
// Function: void setLayer(const int)
// Description:
// Sets the sprite layer.
// Parameters:
// const int layer - The new sprite layer.
//=============================================================================
void Sprite::setLayer(const int layer)
{
	m_layer = layer;
}
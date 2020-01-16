#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite()
	:m_texture(nullptr),
	m_size(),
	m_clip(),
	m_layer(0)
{

}

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
	m_texture->addInstance();
}

Sprite::~Sprite()
{
	m_texture->removeInstance();
	m_texture = nullptr;
}

Sprite& Sprite::operator=(const Sprite& sprite)
{
	if (&sprite == this)
	{
		return *this;
	}

	m_texture = sprite.m_texture;
	m_texture->addInstance();

	m_size = sprite.m_size;
	m_clip = sprite.m_clip;
	m_anchor = sprite.m_anchor;

	// Don't set the sprite layer. It's not allowed. 
	// Set the sprite layer on creation only.

	return *this;
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
// Function: void setSize(const Rectangle&)
// Description: 
// Sets the sprite size to the new size and makes sure the clip
// isn't larger than the size of the sprite.
// Parameters: 
// const Rectangle& size - The new size of the sprite.
//=============================================================================
void Sprite::setSize(const Rectangle& size)
{
	m_size = size;

	if (m_size.getWidth() < m_clip.getWidth())
	{
		m_clip.setWidth(m_size.getWidth());
	}

	if (m_size.getHeight() < m_clip.getHeight())
	{
		m_clip.setHeight(m_size.getHeight());
	}
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

//=============================================================================
// Function: BB_fstream& operator<<(BB_fstream&, const Sprite&)
// Description: 
// Reads the sprite information into the file stream. 
// NOTE: THIS DOESN'T SAVE ANY TEXTURE INFORMATION.
// Parameters: 
// BB_fstream& bbstream - The file stream to read into.
// const Sprite& sprite - The sprite to save the information of.
// Output: 
// BB_fstream&
// Returns the modified file stream.
//=============================================================================
BB_fstream& operator<<(BB_fstream& bbstream,
	const Sprite& sprite)
{
	if (sprite.getTexture())
	{
		bbstream << sprite.getTexture()->getPath();
	}

	bbstream << sprite.getSize();
	bbstream << sprite.getClip();
	bbstream << sprite.getAnchor();
	bbstream << sprite.getLayer();

	return bbstream;
}

//=============================================================================
// Function: BB_fstream& operator>>(BB_fstream&, Sprite&)
// Description: 
// Reads information for the sprite in from the file stream.
// NOTE: THIS DOESN'T LOAD/MODIFY TEXTURE DATA.
// Parameters: 
// BB_fstream& bbstream - The file stream to read from.
// Sprite& sprite - The sprite to read into.
// Output: 
// BB_fstream&
// Returns the modified file stream.
//=============================================================================
BB_fstream& operator>>(BB_fstream& bbstream,
	Sprite& sprite)
{
	Rectangle size;
	Rectangle clip;
	Vector2D anchor;
	int layer = 0;

	bbstream >> size;
	bbstream >> clip;
	bbstream >> anchor;
	bbstream >> layer;

	sprite.setSize(size);
	sprite.setClip(clip);
	sprite.setAnchor(anchor);
	sprite.setLayer(layer);

	return bbstream;
}
#include "RenderSystem.h"
#include "Sprite.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Window.h"
#include "Camera2D.h"

RenderSystem::RenderSystem(const Rectangle& gridBounds,
	const int maxItems,
	const int maxLevels)
	:m_renderGrid(NULL),
	m_cameras(NULL),
	m_cameraCount(0),
	m_activeCamera(-1)
{

	m_renderGrid = new Quadtree*[LAYER_TOTAL];

	for (int i = 0; i < (int)LAYER_TOTAL; i++)
	{
		m_renderGrid[i] = 
			new Quadtree(maxItems, maxLevels, 0, gridBounds, NULL);
	}
}

RenderSystem::~RenderSystem()
{
	cleanUp();
}

//=============================================================================
// Function: const Animator* getAnimator(const int) const
// Description:
// Gets the animator with the specified ID.
// Parameters:
// const int animatorID - The ID of the animator to get.
// Output:
// const Animator*
// On success - Returns a pointer to the found Animator.
// On failure - Returns NULL.
//=============================================================================
const Animator* RenderSystem::getAnimator(const int animatorID) const
{
	Animator *animator = NULL;

	auto mit = m_animators.find(animatorID);

	if (mit != m_animators.end())
	{
		animator = mit->second;
	}

	return animator;
}

//=============================================================================
// Function: const Sprite* getSprite(const int) const
// Description:
// Gets the sprite with the specified id.
// Parameters:
// const int spriteID - The id of the sprite to get.
// Output:
// const Sprite*
// On success - Returns a pointer to a constant sprite object.
// On failure - Returns NULL.
//=============================================================================
const Sprite* RenderSystem::getSprite(const int spriteID) const
{
	Sprite *sprite = NULL;

	auto mit = m_sprites.find(spriteID);

	if (mit != m_sprites.end())
	{
		sprite = mit->second;
	}

	return sprite;
}

//=============================================================================
// Function: Camera2D* getCamera(const int) const
// Description:
// Gets the camera at the specified index.
// Parameters:
// const int index - The index of the camera.
// Output:
// Camera2D*
// On success - Returns the camera found.
// On failure - Returns NULL.
//=============================================================================
Camera2D* RenderSystem::getCamera(const int index) const
{
	Camera2D *camera = NULL;

	if (0 <= index && index < m_cameraCount)
	{
		camera = m_cameras[index];
	}

	return camera;
}

//=============================================================================
// Function: const Animator* createAnimator(const int,
// const string,
// const string,
// const float)
// Description:
// Creates an animator for the specified ID, setting its animation
// set to the provided animation set, and its animation to the 
// specified animation.
// Parameters:
// const int animatorID - The ID of the animator to create.
// const string animationSetName - The name of the animation set
// to use for the animator.
// const string animationName - The name of the animation to use.
// const float speed - The speed of the animation.
// Output:
// const Animator*
// On success - Returns a pointer to the created Animator object.
// On failure - Returns NULL.
//=============================================================================
const Animator* RenderSystem::createAnimator(const int animatorID,
	const string animationSetName,
	const string animationName,
	const float speed)
{
	Animator *animator = NULL;

	auto mit = m_animators.find(animatorID);

	if (mit != m_animators.end())
	{
		animator = mit->second;
	}
	else
	{
		const AnimationSet *set = ResourceManager::getAnimationSet(animationSetName);

		if (set)
		{
			animator = new Animator(set);
			animator->setAnimation(animationName);
			animator->setSpeed(speed);

			m_animators.insert(std::make_pair(animatorID, animator));
		}
	}

	return animator;
}

//=============================================================================
// Function: const Sprite* createSprite(const int,
// const string,
// const Rectangle&,
// const Rectangle&,
// const Vector2D&,
// const int)
// Description:
// Creates a sprite object with the specified parameters and returns
// it.
// Parameters:
// const int spriteID - The id of the sprite to add.
// const string spritePath - The path to the sprite file.
// const Rectangle& size - The size and location of the sprite.
// const Rectangle& clip - The clip to use for the texture.
// const Vector2D& anchor - The anchor point to use.
// const int layer - The layer to set. Has built in error checking.
// Output:
// const Sprite*
// On success - Returns the created sprite object.
// On failure - Returns NULL.
//=============================================================================
const Sprite* RenderSystem::createSprite(const int spriteID,
	const string spritePath,
	const Rectangle& size,
	const Rectangle& clip,
	const Vector2D& anchor,
	const int layer)
{
	Sprite *sprite = NULL;

	auto mit = m_sprites.find(spriteID);

	if (mit != m_sprites.end())
	{
		sprite = mit->second;
	}
	else
	{
		Texture *texture = ResourceManager::getTexture(spritePath);

		if (texture)
		{		
			int workingLayer = layer;

			if (workingLayer < 0)
			{
				workingLayer = 0;
			}
			else if ((int)LAYER_TOTAL <= workingLayer)
			{
				workingLayer = (int)LAYER_TOTAL - 1;
			}

			sprite = 
				new Sprite(texture, size, clip, anchor, workingLayer);

			if (m_renderGrid[workingLayer])
			{
				m_renderGrid[workingLayer]->addEntity(EntityData(spriteID, sprite->getSize()));
			}

			m_sprites.insert(std::make_pair(spriteID, sprite));
		}
	}

	return sprite;
}

//=============================================================================
// Function: void setSpriteLayer(const int, const int)
// Description:
// Removes the sprite from it's previous layer and moves the
// sprite to the new layer.
// Parameters:
// const int spriteID - The id of the sprite.
// const int layer - The new sprite layer.
//=============================================================================
void RenderSystem::setSpriteLayer(const int spriteID,
	const RenderLayer layer)
{
	auto mit = m_sprites.find(spriteID);

	if (mit != m_sprites.end())
	{
		Sprite *sprite = mit->second;

		if (sprite)
		{
			int workingLayer = layer;

			if (workingLayer < 0)
			{
				workingLayer = 0;
			}
			else if ((int)LAYER_TOTAL <= workingLayer)
			{
				workingLayer = (int)LAYER_TOTAL - 1;
			}

			if (sprite->getLayer() != workingLayer)
			{
				m_renderGrid[sprite->getLayer()]->removeEntity(EntityData(spriteID, sprite->getSize()));
				m_renderGrid[workingLayer]->addEntity(EntityData(spriteID, sprite->getSize()));

				sprite->setLayer(workingLayer);
			}
		}
	}
}

//=============================================================================
// Function: void addCamera(Camera2D *camera)
// Description:
// Adds a camera to the list of cameras.
// Parameters:
// Camera2D *camera - The camera to add.
//=============================================================================
void RenderSystem::addCamera(Camera2D *camera)
{
	if (m_cameras == NULL)
	{
		m_cameraCount++;
		m_cameras = new Camera2D*[m_cameraCount];

		m_cameras[m_cameraCount - 1] = camera;

		m_activeCamera = m_cameraCount - 1;
	}
	else
	{
		m_cameraCount++;

		Camera2D **temp = new Camera2D*[m_cameraCount];

		for (int i = 0; i < m_cameraCount - 1; i++)
		{
			temp[i] = m_cameras[i];
		}

		temp[m_cameraCount - 1] = camera;

		m_cameras = temp;
		temp = NULL;
	}
}

//=============================================================================
// Function: void clear()
// Description:
// Clears the renderer.
//=============================================================================
void RenderSystem::clear()
{
	Renderer *renderer = ResourceManager::getRenderer();

	if (renderer)
	{
		renderer->clear();
	}
}

//=============================================================================
// Function: void update()
// Description:
// Updates the system and it's components.
//=============================================================================
void RenderSystem::update()
{
	renderSprites();
	renderUI();

	Renderer *renderer = ResourceManager::getRenderer();

	if (renderer)
	{
		renderer->update();
	}
}

//=============================================================================
// Function: void pauseAnimations()
// Description:
// Pauses the non-UI animators.
//=============================================================================
void RenderSystem::pauseAnimations()
{
	m_animatorsPaused = true;
}

//=============================================================================
// Function: void unpauseAnimations()
// Description:
// Unpauses the non-UI animators.
//=============================================================================
void RenderSystem::unpauseAnimations()
{
	m_animatorsPaused = false;
}

//=============================================================================
// Function: void renderSprites()
// Description:
// Renders all of the visible sprites.
//=============================================================================
void RenderSystem::renderSprites()
{
	Renderer *renderer = ResourceManager::getRenderer();

	if (renderer)
	{
		for (int i = 0; i < (int)LAYER_UI_BACKGROUND; i++)
		{
			if (m_renderGrid[i])
			{
				Vector2D offset(0.0f, 0.0f);
				float xScale = 1.0f;
				float yScale = 1.0f;

				Rectangle viewPort(Vector2D(0.0f, 0.0f), 1000, 1000);

				if (m_activeCamera != -1)
				{
					Camera2D *camera = m_cameras[m_activeCamera];

					if (camera)
					{
						offset = camera->getPosition();

						xScale = camera->getCurrentScaleX();
						yScale = camera->getCurrentScaleY();
						
						offset.m_x *= xScale;
						offset.m_y *= yScale;

						offset.m_x -= camera->getSize().getWidth() / 2;
						offset.m_y -= camera->getSize().getHeight() / 2;

 						viewPort.setWidth((camera->getSize().getWidth()) / xScale);
						viewPort.setHeight((camera->getSize().getHeight()) / yScale);

						viewPort.setCenter(camera->getPosition().m_x, camera->getPosition().m_y);
					}
				}
				else if (ResourceManager::getWindow())
				{
					viewPort.setWidth(ResourceManager::getWindow()->getWidth());
					viewPort.setHeight(ResourceManager::getWindow()->getHeight());

					viewPort.setCenter((float)(viewPort.getWidth() / 2), (float)(viewPort.getHeight() / 2));
				}

				viewPort.setWidth(viewPort.getWidth() + 32);
				viewPort.setHeight(viewPort.getHeight() + 32);

				std::vector<EntityData> renderItems
					= m_renderGrid[i]->search(viewPort);

				for (unsigned int i = 0; i < renderItems.size(); i++)
				{
					const Sprite *sprite = getSprite(renderItems[i].m_id);

					if (sprite)
					{
						Rectangle dest = sprite->getSize();
						
						dest.setWidth(dest.getWidth() * xScale);
						dest.setHeight(dest.getHeight() * yScale);
						
						float centerX = round(dest.getCenter().m_x * xScale - offset.m_x);
						float centerY = round(dest.getCenter().m_y * yScale - offset.m_y);

						dest.setCenter(centerX, centerY);

						Rectangle clip = sprite->getClip();

						Animator *animator = getAnimator(renderItems[i].m_id);

						if (animator)
						{
							if (animator->getFrame())
							{
								clip = *animator->getFrame();

								if (!m_animatorsPaused &&
									animator->getActive())
								{
									animator++;
								}
							}
						}

						renderer->drawTexture(*sprite->getTexture(),
							clip,
							dest,
							sprite->getAnchor());
					}
				}
			}
		}
	}
}

//=============================================================================
// Function: void renderUI()
// Description:
// Renders all of the UI layers.
//=============================================================================
void RenderSystem::renderUI()
{
	Renderer *renderer = ResourceManager::getRenderer();

	if (renderer)
	{
		for (int i = (int)LAYER_UI_BACKGROUND; i < (int)LAYER_TOTAL; i++)
		{
			if (m_renderGrid[i])
			{
				Rectangle viewPort;

				if (0 < m_cameraCount)
				{
					Camera2D *camera = m_cameras[m_activeCamera];

					int width = (int)round(camera->getSize().getWidth() / camera->getCurrentScaleX());
					int height = (int)round(camera->getSize().getHeight() / camera->getCurrentScaleY());

					viewPort.setCenter(camera->getPosition());
					viewPort.setWidth(width);
					viewPort.setHeight(height);
				}
				else
				{
					Window *window = ResourceManager::getWindow();

					Vector2D center((float)window->getWidth() / 2.0f,
						(float)window->getHeight() / 2.0f);

					viewPort.setWidth(window->getWidth());
					viewPort.setHeight(window->getHeight());
					viewPort.setCenter(center);
				}

				std::vector<EntityData> data =
					m_renderGrid[i]->search(viewPort);

				for (unsigned int i = 0; i < data.size(); i++)
				{
					const Sprite *sprite = getSprite(data[i].m_id);

					if (sprite)
					{
						Rectangle clip = sprite->getClip();

						Animator *animator = getAnimator(data[i].m_id);

						if (animator)
						{
							if (animator->getFrame())
							{
								clip = *animator->getFrame();

								if (animator->getActive())
								{
									animator++;
								}
							}
						}

						renderer->drawTexture(*sprite->getTexture(),
							clip,
							sprite->getSize(),
							sprite->getAnchor());
					}
				}
			}
		}
	}
}

//=============================================================================
// Function: Animator* getAnimator(const int)
// Description:
// Gets the animator with the specified ID. This is the version
// which allows for internal editing.
// Parameters:
// const int animatorID - The ID of the animator to get.
// Output:
// Animator*
// On success - Returns a pointer to the found Animator.
// On failure - Returns NULL.
//=============================================================================
Animator* RenderSystem::getAnimator(const int animatorID)
{
	Animator *animator = NULL;

	auto mit = m_animators.find(animatorID);

	if (mit != m_animators.end())
	{
		animator = mit->second;
	}

	return animator;
}

//=============================================================================
// Function: void cleanUp()
// Description:
// Cleans up all of the components and layers, deleting all of them
// and freeing up the memory.
//=============================================================================
void RenderSystem::cleanUp()
{
	if (m_renderGrid)
	{
		delete[] m_renderGrid;
	}

	if (m_cameras)
	{
		delete[] m_cameras;
		m_cameraCount = 0;
	}

	auto spriteIt = m_sprites.begin();

	while (spriteIt != m_sprites.end())
	{
		delete spriteIt->second;

		spriteIt = m_sprites.erase(spriteIt);

		if (spriteIt != m_sprites.end())
		{
			spriteIt++;
		}
	}

	auto animatorIt = m_animators.begin();

	while (animatorIt != m_animators.end())
	{
		delete animatorIt->second;

		animatorIt = m_animators.erase(animatorIt);

		if (animatorIt != m_animators.end())
		{
			animatorIt++;
		}
	}
}
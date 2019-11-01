#pragma once
//==========================================================================================
// File Name: RenderSystem.h
// Author: Brian Blackmon
// Date Created: 10/25/2019
// Purpose: 
// Handles all the rendering and resource management for render
// items.
//==========================================================================================
#include <map>
#include <string>
#include "Quadtree.h"
#include "Rectangle.h"

typedef std::string string;

class Animator;
class AnimationSet;
class Sprite;
class Camera2D;

class RenderSystem
{
public:
	enum RenderLayer
	{
		LAYER_BACKGROUND_0,
		LAYER_BACKGROUND_1,
		LAYER_MIDGROUND_0,
		LAYER_MIDGROUND_1,
		LAYER_FOREGROUND_0,
		LAYER_FOREGROUND_1,
		LAYER_UI_BACKGROUND,
		LAYER_UI_MIDGROUND,
		LAYER_UI_FOREGROUND,
		LAYER_TOTAL
	};

	RenderSystem(const Rectangle& gridBounds,
		const int maxItems,
		const int maxLevels);
	~RenderSystem();

	const Animator* getAnimator(const int animatorID) const;
	const Sprite* getSprite(const int spriteID) const;

	Camera2D* getCamera(const int index) const;

	const Animator* createAnimator(const int animatorID,
		const string animationSetName,
		const string animationName,
		const float speed);

	const Sprite* createSprite(const int spriteID,
		const string spritePath,
		const Rectangle& size,
		const Rectangle& clip,
		const Vector2D& anchor,
		const int layer);

	void setSpriteLayer(const int spriteID, 
		const RenderLayer layer);

	void addCamera(Camera2D *camera);

	void clear();
	void update();
	void pauseAnimations();
	void unpauseAnimations();
	void renderSprites();
	void renderUI();

private:
	std::map<int, Animator*> m_animators;
	std::map<int, Sprite*> m_sprites;

	Quadtree **m_renderGrid;
	Camera2D **m_cameras;

	int m_cameraCount;
	int m_activeCamera;
	
	bool m_animatorsPaused;

	Animator* getAnimator(const int animatorID);
	void cleanUp();
};


#pragma once
//==========================================================================================
// File Name: Game.h
// Author: Brian Blackmon
// Date Created: 10/19/2019
// Purpose: 
// It's the game. It's how everything runs.
//==========================================================================================
#include <string>

class Window;
class Renderer;
class TextureCache;
class RenderSystem;
class PhysicsSystem;
class InputSystem;

typedef std::string string;

class Game
{
public:
	enum GameState
	{
		GAMESTATE_RUNNING,
		GAMESTATE_PAUSED,
		GAMESTATE_QUIT
	};

	Game(const string loadPath);
	~Game();

	void loop();

private:
	Window *m_window;
	Renderer *m_renderer;
	TextureCache *m_textureCache;
	RenderSystem *m_renderSystem;
	PhysicsSystem *m_physicsSystem;
	InputSystem *m_inputSystem;

	GameState m_gameState;

	void renderClear();
	void update(const float delta);
	void renderUpdate();

	bool init(const string loadPath);
	void cleanUp();
};


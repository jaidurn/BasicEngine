#include "Game.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Window.h"
#include "TextureCache.h"
#include "SettingIO.h"
#include "RenderSystem.h"
#include "LogLocator.h"
#include "Camera2D.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "InputDevice.h"

Game::Game(const string loadPath)
	:m_window(NULL),
	m_renderer(NULL),
	m_textureCache(NULL),
	m_renderSystem(NULL),
	m_physicsSystem(NULL),
	m_inputSystem(nullptr),
	m_gameState(GAMESTATE_RUNNING)
{
	if (!init(loadPath))
	{
		m_gameState = GAMESTATE_QUIT;
	}
}

Game::~Game()
{
	cleanUp();
}

//=============================================================================
// Function: void loop()
// Description:
// Handles the game loop.
//=============================================================================
void Game::loop()
{
	SDL_Event e;

	while (m_gameState != GAMESTATE_QUIT)
	{
		static Uint32 ticks = SDL_GetTicks();
		ticks = SDL_GetTicks();

		while (SDL_PollEvent(&e))
		{
			m_inputSystem->update(e);

			switch (e.type)
			{
			case SDL_QUIT:
			{
				m_gameState = GAMESTATE_QUIT;
				break;
			}
			}
		}

		static Uint32 renderTicks = 0;

		renderClear();
		update((float)renderTicks / 1000.0f);

		CollisionBox *a = m_physicsSystem->getCollisionBox(0);
		CollisionBox *b = m_physicsSystem->getCollisionBox(1);
		CollisionBox *c = m_physicsSystem->getCollisionBox(2);
		CollisionBox *d = m_physicsSystem->getCollisionBox(3);
		Velocity *vel = m_physicsSystem->getVelocity(1);

		m_renderer->drawRect(a->getBox(), SDL_Color{ 255, 0, 0, 255 });
		m_renderer->drawRect(b->getBox(), SDL_Color{ 255, 0, 0, 255 });
		m_renderer->drawRect(c->getBox(), SDL_Color{ 255, 0, 0, 255 });
		m_renderer->drawRect(d->getBox(), SDL_Color{ 255, 0, 0, 255 });
		m_renderer->drawLine(Line(b->getPosition(), b->getPosition() + vel->getDirection()), SDL_Color{ 0, 255, 0, 255 });
		m_renderer->drawLine(Line(b->getPosition(), getMidPoint(Line(b->getBox().getTopLeft(), b->getBox().getTopRight()))), SDL_Color{ 0, 0, 255, 255 });

		Uint32 logicTicks = SDL_GetTicks() - ticks;
		LogLocator::getLog().log("Logic Ticks: " + std::to_string(logicTicks));
		renderUpdate();
		renderTicks = SDL_GetTicks() - ticks - logicTicks;
		LogLocator::getLog().log("Render Ticks: " + std::to_string(renderTicks));
	}
}

//=============================================================================
// Function: void renderClear()
// Description:
// Clears the renderer.
//=============================================================================
void Game::renderClear()
{
	if (m_renderSystem)
	{
		m_renderSystem->clear();
	}
}

//=============================================================================
// Function: void update(const float)
// Description:
// Updates the game logic.
// Parameters:
// const float delta - The time passed since last update.
//=============================================================================
void Game::update(const float delta)
{
	if (m_gameState == GAMESTATE_RUNNING)
	{
		InputDevice *device = m_inputSystem->getDevice(-2);

		if (device)
		{
			if (device->buttonPressed(BTN_ANALOG_LEFT))
			{
				Velocity *vel = m_physicsSystem->getVelocity(1);

				vel->addVelocity(Vector2D(-1.0f, 0.0f));
			}

			if (device->buttonPressed(BTN_ANALOG_RIGHT))
			{
				Velocity *vel = m_physicsSystem->getVelocity(1);

				vel->addVelocity(Vector2D(1.0f, 0.0f));
			}

			if (device->buttonPressed(BTN_ANALOG_UP))
			{
				Velocity *vel = m_physicsSystem->getVelocity(1);

				vel->addVelocity(Vector2D(0.0f, -1.0f));
			}

			if (device->buttonPressed(BTN_ANALOG_DOWN))
			{
				Velocity *vel = m_physicsSystem->getVelocity(1);

				vel->addVelocity(Vector2D(0.0f, 1.0f));
			}

			if (device->buttonPressed(BTN_BASE_ATTACK_0))
			{
				CollisionBox *box = m_physicsSystem->getCollisionBox(1);
				Rectangle rect = box->getBox();

				rect.setRotation(box->getBox().getRotation() - 2.0f);

				box->setBox(rect);
			}

			if (device->buttonPressed(BTN_BASE_ATTACK_1))
			{
				CollisionBox *box = m_physicsSystem->getCollisionBox(1);
				Rectangle rect = box->getBox();

				rect.setRotation(box->getBox().getRotation() + 2.0f);

				box->setBox(rect);
			}

			if (device->buttonPressed(BTN_SPECIAL_ATTACK_0))
			{
				Velocity *vel = m_physicsSystem->getVelocity(1);

				vel->addVelocity(vel->getDirection() * -2.0f);
			}
		}

		m_physicsSystem->update(delta);
	}
	else if(m_gameState == GAMESTATE_PAUSED)
	{

	}
}

//=============================================================================
// Function: void renderUpdate()
// Description:
// Updates the renderer.
//=============================================================================
void Game::renderUpdate()
{
	if (m_renderSystem)
	{
		m_renderSystem->update();
	}
}

//=============================================================================
// Function: bool init(const string loadPath)
// Description:
// Loads the settings from the specified file.
// Parameters:
// const string loadPath - The file path of the settings file to
// load.
// Output:
// bool
// On success - Returns true.
// On failure - Returns false.
//=============================================================================
bool Game::init(const string loadPath)
{
	bool success = false;

	if (loadPath != "")
	{
		ResourceManager::loadFromFile(loadPath);

		if (ResourceManager::initialized())
		{
			int width = 10000;
			int height = 10000;

			m_renderSystem =
				new RenderSystem(Rectangle(Vector2D((float)(width / 2), (float)(height / 2)), width, height),
					10, 
					10);

			m_physicsSystem =
				new PhysicsSystem(10,
					10,
					Rectangle(Vector2D((float)(width / 2), (float)(height / 2)), width, height));

			m_inputSystem =
				new InputSystem();

			InputDevice* device = m_inputSystem->getDevice(-2);

			if (device)
			{
				device->mapButton(SDLK_w, BTN_ANALOG_UP);
				device->mapButton(SDLK_d, BTN_ANALOG_RIGHT);
				device->mapButton(SDLK_s, BTN_ANALOG_DOWN);
				device->mapButton(SDLK_a, BTN_ANALOG_LEFT);
				device->mapButton(SDLK_q, BTN_BASE_ATTACK_0);
				device->mapButton(SDLK_e, BTN_BASE_ATTACK_1);
				device->mapButton(SDLK_f, BTN_SPECIAL_ATTACK_0);
			}

			m_renderer = ResourceManager::getRenderer();

			Rectangle size(Vector2D(1280.0f / 2.0f, 720.0f / 2.0f), 1280, 720);

			Camera2D *camera = new Camera2D(size, size);

			m_renderSystem->addCamera(camera);

			Rectangle boxBase(Vector2D(100.0f, 100.0f), 32, 32);
			Rectangle otherBox(Vector2D(200.0f, 100.0f), 64, 64);

			otherBox.setRotation(34.0f);

			m_physicsSystem->createCollisionBox(0, boxBase, true);
			m_physicsSystem->createCollisionBox(1, otherBox, true);

			boxBase.setCenter(boxBase.getCenter() + Vector2D(0.0f, 31.0f));
			m_physicsSystem->createCollisionBox(2, boxBase, true);
			
			boxBase.setCenter(boxBase.getCenter() + Vector2D(0.0f, -62.0f));
			m_physicsSystem->createCollisionBox(3, boxBase, true);

			m_physicsSystem->getVelocity(1)->addVelocity(Vector2D(-20.0f, 0.0f));


			success = true;
		}
	}

	return success;
}

void Game::cleanUp()
{
	ResourceManager::cleanUp();
}
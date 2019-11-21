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

Game::Game(const string loadPath)
	:m_window(NULL),
	m_renderer(NULL),
	m_textureCache(NULL),
	m_renderSystem(NULL),
	m_physicsSystem(NULL),
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
			switch (e.type)
			{
			case SDL_QUIT:
			{
				m_gameState = GAMESTATE_QUIT;
				break;
			}
			case SDL_KEYDOWN:
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_LEFT:
				{
					Camera2D *camera = m_renderSystem->getCamera(0);

					if (camera)
					{
						Vector2D newPos = camera->getPosition();
						
						newPos.m_x -= 4.0f;

						camera->setPosition(newPos);
					}
					break;
				}
				case SDLK_RIGHT:
				{
					Camera2D *camera = m_renderSystem->getCamera(0);

					if (camera)
					{
						Vector2D newPos = camera->getPosition();

						newPos.m_x += 4.0f;

						camera->setPosition(newPos);
					}
					break;
				}
				case SDLK_UP:
				{
					Camera2D *camera = m_renderSystem->getCamera(0);

					if (camera)
					{
						Vector2D newPos = camera->getPosition();

						newPos.m_y -= 4.0f;

						camera->setPosition(newPos);
					}
					break;
				}
				case SDLK_DOWN:
				{
					Camera2D *camera = m_renderSystem->getCamera(0);

					if (camera)
					{
						Vector2D newPos = camera->getPosition();

						newPos.m_y += 4.0f;

						camera->setPosition(newPos);
					}
					break;
				}
				case SDLK_KP_PLUS:
				{
					Camera2D *camera = m_renderSystem->getCamera(0);

					if (camera)
					{
						camera->setCurrentScaleX(camera->getCurrentScaleX() + 0.5f);
						camera->setCurrentScaleY(camera->getCurrentScaleY() + 0.5f);
					}
					break;
				}
				case SDLK_KP_MINUS:
				{
					Camera2D *camera = m_renderSystem->getCamera(0);

					if (camera)
					{
						camera->setCurrentScaleX(camera->getCurrentScaleX() - 0.5f);
						camera->setCurrentScaleY(camera->getCurrentScaleY() - 0.5f);
					}
					break;
				}
				case SDLK_q:
				{
					CollisionBox *box = m_physicsSystem->getCollisionBox(1);
					Rectangle rect = box->getBox();

					rect.setRotation(box->getBox().getRotation() - 2.0f);

					box->setBox(rect);

					break;
				}
				case SDLK_e:
				{
					CollisionBox *box = m_physicsSystem->getCollisionBox(1);
					Rectangle rect = box->getBox();

					rect.setRotation(box->getBox().getRotation() + 2.0f);

					box->setBox(rect);

					break;
				}
				case SDLK_w:
				{
					Velocity *vel = m_physicsSystem->getVelocity(1);

					vel->addVelocity(Vector2D(0.0f, -1.0f));

					break;
				}
				case SDLK_d:
				{
					Velocity *vel = m_physicsSystem->getVelocity(1);

					vel->addVelocity(Vector2D(1.0f, 0.0f));

					break;
				}
				case SDLK_s:
				{
					Velocity *vel = m_physicsSystem->getVelocity(1);

					vel->addVelocity(Vector2D(0.0f, 1.0f));

					break;
				}
				case SDLK_a:
				{
					Velocity *vel = m_physicsSystem->getVelocity(1);

					vel->addVelocity(Vector2D(-1.0f, 0.0f));

					break;
				}
				}

				break;
			}
			}
		}

		static Uint32 renderTicks = 0;

		renderClear();
		update((float)renderTicks / 1000.0f);

		CollisionBox *a = m_physicsSystem->getCollisionBox(0);
		CollisionBox *b = m_physicsSystem->getCollisionBox(1);
		Velocity *vel = m_physicsSystem->getVelocity(1);

		m_renderer->drawRect(a->getBox(), SDL_Color{ 255, 0, 0, 255 });
		m_renderer->drawRect(b->getBox(), SDL_Color{ 255, 0, 0, 255 });
		m_renderer->drawLine(Line(b->getPosition(), b->getPosition() + vel->getDirection()), SDL_Color{ 0, 255, 0, 255 });

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

			m_renderer = ResourceManager::getRenderer();

			Rectangle size(Vector2D(1280.0f / 2.0f, 720.0f / 2.0f), 1280, 720);

			Camera2D *camera = new Camera2D(size, size);

			m_renderSystem->addCamera(camera);

			Rectangle boxBase(Vector2D(100.0f, 100.0f), 32, 32);
			Rectangle otherBox(Vector2D(200.0f, 100.0f), 64, 64);

			otherBox.setRotation(34.0f);

			m_physicsSystem->createCollisionBox(0, boxBase, true);
			m_physicsSystem->createCollisionBox(1, otherBox, true);
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
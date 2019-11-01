#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "LogLocator.h"
#include "ConsoleLog.h"
#include "Game.h"

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL Error: "
			<< SDL_GetError()
			<< std::endl;

		return 1; 
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		std::cout << "IMG Error: "
			<< IMG_GetError()
			<< std::endl;

		return 1;
	}

	ConsoleLog *consoleLog = new ConsoleLog();

	LogLocator::setLog(consoleLog);

	Game *game = new Game("settings");

	game->loop();

	delete consoleLog;

	return 0;
}
#include <iostream>
#include <thread>
#include <string>

#include <SDL.h>

#include "Game.h"

using namespace std;

int main(int argc, char** argv)
{
	Game* game = new Game();

	int width = 1366;
	int height = 768;

	game->initialize("DGPP Skelatol", width, height, SDL_WINDOW_INPUT_FOCUS);


	while(game->isRunning())
	{
		game->handleEvents();
		game->update();
		game->render();
	}

	game->cleanUp();
	
	return 0;
}



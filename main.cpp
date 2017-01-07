#include <iostream>
#include <thread>
#include <string>

#include <SDL.h>

#include "Game.h"

using namespace std;

int main(int argc, char** argv)
{
	Game* game = new Game();

	int width = 800; //set to 1250 if 1260, 1300 if 1280. the window is can always fit a certain amount of tiles completely
	int height = 800;

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



#ifndef GAME_H
#define GAME_H
#include <vector>
#include <ctime>
#include "LTimer.h"
#include "Renderer.h"
#include "Camera2D.h"


class Game
{
public:
	Game();
	~Game();
	bool initialize(const char* title, int width, int height, int flags);
	void render();
	void update();
	void handleEvents();
	bool isRunning();
	void cleanUp();
private:
	Camera2D::Camera m_camera;
	Renderer m_renderer;
	bool m_running;
	unsigned int m_lastTime;

	std::vector<SDL_Rect> m_tiles;
};
#endif


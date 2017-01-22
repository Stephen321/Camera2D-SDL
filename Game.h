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
	SDL_Texture* loadTexture(const std::string& path);
	Camera2D::Camera m_camera;
	Renderer m_renderer;
	bool m_running;
	unsigned int m_lastTime;

	std::vector<SDL_Rect> m_rects;
	std::vector<Camera2D::Point> m_points;
	const int POINT_SIZE = 10;
	SDL_Texture* m_background; //© 2012-2013 Julien Jorge <julien.jorge@stuff-o-matic.com>
	SDL_Rect m_backgroundRect;
};
#endif


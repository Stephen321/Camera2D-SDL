#include "Game.h"
#include <iostream>


Game::Game() 
	: m_running(false)
{
}

Game::~Game()
{
}
 

bool Game::initialize(const char* title, int width, int height, int flags)
{
	srand(time(NULL));
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		if (!m_renderer.initialize(&m_camera, title, width, height, flags))
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	m_running = true;

	m_camera.init(width, height, m_renderer.getSDLRenderer());

	Vector2f pos;
	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 30; x++)
		{
			m_tiles.push_back({ (int)pos.x , (int)pos.y , 50, 50 });
			pos.x += 50;
		}
		pos.x = 0;
		pos.y += 50;
	}

	return true;
}


void Game::render()
{
	m_renderer.clear();

	for (const SDL_Rect& rect : m_tiles)
	{
		m_renderer.drawRect(rect, Colour(255, 0, 0, 255));
	}

	m_camera.render();

	m_renderer.present();
}

int x = 0;
int y = 0;
int targetX = 0;
int targetY = 0;

void Game::update()
{
	unsigned int currentTime = LTimer::gameTime();//millis since game started
	unsigned int deltaTime = currentTime - m_lastTime;//time since last update

	m_camera.update(deltaTime / 1000.f);
													
	m_lastTime = currentTime;//save the curent time for next frame
}

void Game::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_running = false;
				break;
			case SDLK_UP:
				m_camera.panY(-1);
				break;
			case SDLK_DOWN:
				m_camera.panY(1);
				break;
			case SDLK_LEFT:
				m_camera.panX(-1);
				break;
			case SDLK_RIGHT:
				m_camera.panX(1);
				break;
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				m_camera.panY(0);
				break;
			case SDLK_DOWN:
				m_camera.panY(0);
				break;
			case SDLK_LEFT:
				m_camera.panX(0);
				break;
			case SDLK_RIGHT:
				m_camera.panX(0);
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y < 0)
			{
			}
			else if (event.wheel.y > 0)
			{
			}
			break;
		case SDL_MOUSEMOTION:
			//TODO: print this out on the screen
			//Camera2D::Point p(event.motion.x, event.motion.y);
			//p = m_camera.screenToWorld(p);
			//std::cout << "Mosue X: " << p.x << " , " << p.y << std::endl;
			break;
		}
	}
}

bool Game::isRunning()
{
	return m_running;
}

void Game::cleanUp()
{
	m_renderer.cleanUp();
	SDL_Quit();
}
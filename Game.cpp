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

	int size = 20;
	Vector2f pos;
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			m_tiles.push_back({ (int)pos.x , (int)pos.y , size, size });
			pos.x += size;
		}
		pos.x = 0;
		pos.y += size;
	}

	m_background = loadTexture("background.bmp");
	int backgroundWidth, backgroundHeight;
	SDL_QueryTexture(m_background, NULL, NULL, &backgroundWidth, &backgroundHeight);
	m_backgroundRect = {0, 0, backgroundWidth, backgroundHeight};
	return true;
}


void Game::render()
{
	m_renderer.clear();

	for (const SDL_Rect& rect : m_tiles)
	{
		m_renderer.drawRect(rect, Colour(255, 0, 0, 255));
		m_renderer.drawOutlineRect(m_camera.worldToScreen(rect), Colour(0, 255, 255, 255));
	}


	m_renderer.drawTexture(m_background, NULL, &m_camera.worldToScreen(m_backgroundRect));
	m_camera.render();

	m_renderer.drawOutlineRect(m_camera.getBounds(), Colour(0, 255, 255, 255));


	m_renderer.drawRect({ 0, 0, 100, 5 }, Colour(255, 0, 0, 255)); //x axis
	m_renderer.drawRect({ 0, 0, 5, 100 }, Colour(0, 255, 0, 255)); //y axis
	m_renderer.drawRect({ 0, 0, 5, 5 }, Colour(0, 0, 255, 255));   //z axis

	m_renderer.present();
}

int x = 0;
int y = 0;
int targetX = 0;
int targetY = 0;

void Game::update()
{
	unsigned int currentTime = LTimer::gameTime();//millis since game started
	float deltaTime = (currentTime - m_lastTime) / 1000.f;//time since last update

	m_camera.update(deltaTime);
													
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
			case SDLK_z:
				m_camera.zoomTo(0.f);
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y < 0)
			{
				m_camera.zoom(1);
			}
			else if (event.wheel.y > 0)
			{
				m_camera.zoom(-1);
			}
			break;
		case SDL_MOUSEMOTION:
			//debug
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
	SDL_DestroyTexture(m_background);
	m_background = NULL;
	m_renderer.cleanUp();
	SDL_Quit();
}

SDL_Texture * Game::loadTexture(const std::string & path)
{
	SDL_Texture* texture = NULL;

	SDL_Surface* surface = SDL_LoadBMP(path.c_str());
	if (surface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(m_renderer.getSDLRenderer(), surface);
		if (texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}

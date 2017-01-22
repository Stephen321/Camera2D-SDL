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


	m_rects.push_back({ 0, 0, RECT_WIDTH, RECT_WIDTH });
	m_rects.push_back({ 100, 0, RECT_WIDTH, RECT_WIDTH });
	m_rects.push_back({ -250, -100, RECT_WIDTH, RECT_WIDTH });
	m_rects.push_back({ 500, 50, RECT_WIDTH, RECT_WIDTH });
	m_rects.push_back({ 200, 300, RECT_WIDTH, RECT_WIDTH });

	m_points.push_back(Camera2D::Point(0.f, 0.f));
	m_points.push_back(Camera2D::Point(150.f, 0.f));
	m_points.push_back(Camera2D::Point(0.f, 150.f));
	m_points.push_back(Camera2D::Point(300.f, 0.f));
	m_points.push_back(Camera2D::Point(0.f, -300.f));

	m_background = loadTexture("background.bmp");
	int backgroundWidth, backgroundHeight;
	SDL_QueryTexture(m_background, NULL, NULL, &backgroundWidth, &backgroundHeight);
	m_backgroundRect = {0, 0, backgroundWidth, backgroundHeight};


	m_camera.setZoomMinMax(-1, -1.f);
	return true;
}


void Game::render()
{
	m_renderer.clear();


	m_renderer.drawTexture(m_background, NULL, &m_camera.worldToScreen(m_backgroundRect));
	m_camera.render();


	
	m_renderer.drawOutlineRect(m_camera.getBounds(), Colour(0, 255, 255, 255));

	for (const SDL_Rect& rect : m_rects)
	{
		m_renderer.drawRect(rect, Colour(255, 0, 0, 255));
	}

	m_renderer.drawRect({ 0, 0, 100, 5 }, Colour(255, 0, 0, 255)); //x axis
	m_renderer.drawRect({ 0, 0, 5, 100 }, Colour(0, 255, 0, 255)); //y axis
	m_renderer.drawRect({ 0, 0, 5, 5 }, Colour(0, 0, 255, 255));   //z axis

	for (const Camera2D::Point& point : m_points)
	{
		SDL_Rect r = { (int)(point.x - POINT_SIZE * 0.5f), (int)(point.y - POINT_SIZE * 0.5f), POINT_SIZE, POINT_SIZE };
		m_renderer.drawRect(r, Colour(255, 255, 255, 255));
	}


	m_renderer.present();
}

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
				m_camera.zoomTo(5.f);
				break;
			case SDLK_x:
				m_camera.zoomToFit(m_points, false);
				break;
			case SDLK_c:
				m_camera.zoomToFit(m_rects);
				break;
			case SDLK_v:
				m_camera.setZoomMinMax(-1.f, 0.5f);
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEWHEEL:
			m_camera.resetZoomRatio();
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

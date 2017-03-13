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

	//init with size and renderer
	m_camera.init(width, height, m_renderer.getSDLRenderer());
	m_camera.setCentre(1000.f, 700.f);

	m_background = loadTexture("assets/background.png");
	int backgroundWidth, backgroundHeight;
	SDL_QueryTexture(m_background, NULL, NULL, &backgroundWidth, &backgroundHeight);
	m_backgroundRect = {0, 0, backgroundWidth, backgroundHeight};

	m_attractorTexture = loadTexture("assets/attractor.png");
	m_repulsorTexture = loadTexture("assets/repulsor.png");

	//set unlimited min and max zoom
	m_camera.setZoomMinMax(-1, -1.f);

	//create and add parallax effect named "parallax"
	Camera2D::ParallaxEffect pe;
	// add layer "stars" with textures found in assets/starts, scroll multiplier of 0.2, default to loading in 3 sections as that is how many there is
	//if set to less than 3 then copy of texture is used
	pe.addLayer("stars", Camera2D::Layer("assets/stars", 0.2f));
	pe.addLayer("mountains", Camera2D::Layer("assets/mountains", 0.6f, 7));
	m_camera.addEffect(pe, "parallax");

	//create shake effect
	Camera2D::ShakeEffect se;
	float duration = 2.f;
	float speed = 2.f;
	float magnitude = 1.f;
	se.setProps(duration, speed, magnitude);
	m_camera.addEffect(se, "shake");

	m_camera.restrictCentre({ 0,0, 1000, 1000 });
	return true;
}


void Game::render()
{
	m_renderer.clear();

	//default background
	m_renderer.drawTexture(m_background, NULL, &m_backgroundRect);

	//parallax background
	m_camera.render();

	//attractors
	for (const SDL_Rect& rect : m_attractorRects)
	{
		m_renderer.drawTexture(m_attractorTexture, NULL, &rect);
	}
	//repulsors
	for (const SDL_Rect& rect : m_repulsorRects)
	{
		m_renderer.drawTexture(m_repulsorTexture, NULL, &rect);
	}
	//rects
	for (const SDL_Rect& rect : m_rects)
	{
		m_renderer.drawRect(rect, Colour(0, 0, 255, 255));
	}
	//points
	for (const Camera2D::Point& point : m_points)
	{
		SDL_Rect r = { (int)(point.x - POINT_SIZE * 0.5f), (int)(point.y - POINT_SIZE * 0.5f), POINT_SIZE, POINT_SIZE };
		m_renderer.drawRect(r, Colour(255, 0, 0, 255));
	}

	m_renderer.present();
}

void Game::update()
{
	//get delta time
	unsigned int currentTime = LTimer::gameTime();//millis since game started
	float deltaTime = (currentTime - m_lastTime) / 1000.f;//time since last update

	//update camera
	if (deltaTime < 60.f / 1000.f)
		m_camera.update(deltaTime);
				
	//remember time
	m_lastTime = currentTime;//save the curent time for next frame
}

void Game::handleEvents()
{
	SDL_Event event;
	//update our view of the keyboard state
	const Uint8 * keyboardState = SDL_GetKeyboardState(NULL);
	
	//poll events 1 by 1 
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
			case SDLK_y:
				m_camera.setMaxVelocity(m_camera.getMaxVelocity() + 1.f);
				std::cout << "Max velocity: " << m_camera.getMaxVelocity() << std::endl;
				break;
			case SDLK_g:
				m_camera.setMaxVelocity(m_camera.getMaxVelocity() - 1.f);
				std::cout << "Max velocity: " << m_camera.getMaxVelocity() << std::endl;
				break;
			case SDLK_u:
				m_camera.setAccelerationRate(m_camera.getAccelerationRate() + 1.f);
				std::cout << "Acceleration rate: " << m_camera.getAccelerationRate() << std::endl;
				break;
			case SDLK_h:
				m_camera.setAccelerationRate(m_camera.getAccelerationRate() - 1.f);
				std::cout << "Acceleration rate: " << m_camera.getAccelerationRate() << std::endl;
				break;
			case SDLK_o: //increase parallax stars scroll speed
			{
				Camera2D::ParallaxEffect * pe = m_camera.findParallax("parallax");
				if (pe != nullptr)
				{
					Camera2D::Layer* layer = pe->getLayer("mountains");
					if (layer != nullptr)
					{
						layer->setScrollMultiplier(layer->getScrollMultiplier() + 0.1f);
						std::cout << "Mountain scroll multiplier rate: " << layer->getScrollMultiplier() << std::endl;
					}
				}
				break;
			}
			case SDLK_k: //decrease parallax stars scroll speed
			{
				Camera2D::ParallaxEffect * pe = m_camera.findParallax("parallax");
				if (pe != nullptr)
				{
					Camera2D::Layer* layer = pe->getLayer("mountains");
					if (layer != nullptr)
					{
						layer->setScrollMultiplier(layer->getScrollMultiplier() - 0.1f);
						std::cout << "Mountain scroll multiplier rate: " << layer->getScrollMultiplier() << std::endl;
					}
				}
				break;
			}
			case SDLK_i: //increase shake magnitude
			{
				Camera2D::ShakeEffect * shakeEffect = m_camera.findShake("shake");
				if (shakeEffect != nullptr)
				{
					shakeEffect->setMagnitude(shakeEffect->getMagnitude() + 0.1f);
					std::cout << "Shake magnitude: " << shakeEffect->getMagnitude() << std::endl;
				}
				break;
			}
			case SDLK_j: //decrease shake magnitude
			{
				Camera2D::ShakeEffect * shakeEffect = m_camera.findShake("shake");
				if (shakeEffect != nullptr)
				{
					shakeEffect->setMagnitude(shakeEffect->getMagnitude() - 0.1f);
					std::cout << "Shake magnitude: " << shakeEffect->getMagnitude() << std::endl;
				}
				break;
			}
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_1:
				m_camera.setAllowedHorizontal(!m_camera.getAllowedHorizontal());
				std::cout << "Set Allowed Horizontal to: " << ((m_camera.getAllowedHorizontal()) ? "true" : "false") << std::endl;
				break;
			case SDLK_2:
				m_camera.setAllowedVertical(!m_camera.getAllowedVertical());
				std::cout << "Set Allowed Vertical to: " << ((m_camera.getAllowedVertical()) ? "true" : "false") << std::endl;
				break;
			case SDLK_3:
				if (m_camera.getAllowedHorizontal() == false && m_camera.getAllowedVertical() == false)
				{
					m_camera.unlockMotion();
					std::cout << "Unlocked" << std::endl;
				}
				else
				{
					m_camera.lockMotion();
					std::cout << "Locked" << std::endl;
				}
				break;
			case SDLK_LEFTBRACKET:
				m_camera.zoomTo(3.f);
				std::cout << "Zooming to 3.f" << std::endl;
				break;
			case SDLK_RIGHTBRACKET:
				m_camera.zoomTo(0.f);
				std::cout << "Zooming to 0.f" << std::endl;
				break;
			case SDLK_z:
				m_camera.zoomToFit(m_rects);
				std::cout << "Zoom to fit rectangles" << std::endl;
				break;
			case SDLK_x:
				m_camera.zoomToFit(m_points);
				std::cout << "Zoom to fit points" << std::endl;
				break;
			case SDLK_c:
				m_camera.zoomToFit(m_points, false);
				std::cout << "Zoom to fit points without preserving aspect ratio" << std::endl;
				break;
			case SDLK_a:
			{
				int x;
				int y;
				SDL_GetMouseState(&x, &y);

				Camera2D::Attractor attractor;
				Camera2D::Vector2 v(x, y);
				v = m_camera.screenToWorld(v);
				attractor.setProps(v);
				m_camera.addInfluencer(attractor, "" + m_attractorRects.size());
				m_attractorRects.push_back({ (int)(v.x - INFLUENCER_RANGE * 0.5f), (int)(v.y - INFLUENCER_RANGE * 0.5f), INFLUENCER_RANGE , INFLUENCER_RANGE });
				std::cout << "Create Attractor at : (" << x << ", " << y << ")" << std::endl;
				break;
			}
			case SDLK_r:
			{
				int x;
				int y;
				SDL_GetMouseState(&x, &y);

				Camera2D::Repulsor repulsor;
				Camera2D::Vector2 v(x, y);
				v = m_camera.screenToWorld(v);
				repulsor.setProps(v);
				m_camera.addInfluencer(repulsor, "" + m_repulsorRects.size());
				m_repulsorRects.push_back({ (int)(v.x - INFLUENCER_RANGE * 0.5f), (int)(v.y - INFLUENCER_RANGE * 0.5f), INFLUENCER_RANGE , INFLUENCER_RANGE });
				std::cout << "Create Repulsor at : (" << x << ", " << y << ")" << std::endl;
				break;
			}
			case SDLK_v:
				m_camera.setZoomMinMax(-1.f, 0.5f);
				std::cout << "set zoom min to unlimited and max to 0.5f" << std::endl;
				break;
			case SDLK_s:
				if (m_camera.findEffect("shake")->getEnabled())
				{
					m_camera.endEffect("shake");
					std::cout << "End shake" << std::endl;
				}
				else
				{
					m_camera.startEffect("shake");
					std::cout << "Start shake" << std::endl;
				}
				break;
			case SDLK_p:
				if (m_camera.findEffect("parallax")->getEnabled())
				{
					m_camera.endEffect("parallax");
					std::cout << "End parallax" << std::endl;
				}
				else
				{
					m_camera.startEffect("parallax");
					std::cout << "Start parallax" << std::endl;
				}
				break;
			case SDLK_q:
				m_rects.clear();
				m_points.clear();
				std::cout << "Clear rects and points" << std::endl;
				break;
			case SDLK_w:
				m_attractorRects.clear();
				m_camera.removeAllInfluencer(Camera2D::Influencer::Type::Attractor);
				m_repulsorRects.clear();
				m_camera.removeAllInfluencer(Camera2D::Influencer::Type::Repulsor);
				std::cout << "Clear attractors and repulsors" << std::endl;
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
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				Camera2D::Point point((int)(event.button.x - RECT_WIDTH * 0.5f), (int)(event.button.y - RECT_HEIGHT * 0.5f));
				point = m_camera.screenToWorld(point);
				m_rects.push_back({ (int)point.x, (int)point.y, RECT_WIDTH, RECT_HEIGHT });

				std::cout << "Create rect at (" << point.x << ", " << point.y << ")" << std::endl;
				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				Camera2D::Point point((int)(event.button.x), (int)(event.button.y));
				point = m_camera.screenToWorld(point);
				m_points.push_back(point);
				std::cout << "Create point at (" << point.x << ", " << point.y << ")" << std::endl;
				break;
			}
			}
		}
	}

	if (keyboardState[SDL_SCANCODE_RIGHT])
	{
		m_camera.panX(1);
	}
	else if (keyboardState[SDL_SCANCODE_LEFT])
	{
		m_camera.panX(-1);
	}
	if (keyboardState[SDL_SCANCODE_UP])
	{
		m_camera.panY(-1);
	}
	else if (keyboardState[SDL_SCANCODE_DOWN])
	{
		m_camera.panY(1);
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

	SDL_Surface* surface = IMG_Load(path.c_str());
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

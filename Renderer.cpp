#include "Renderer.h"

Renderer::Renderer()
	: m_window(NULL)
	, m_renderer(NULL)
{
}


bool Renderer::initialize(const Camera2D::Camera* camera, const char* title, int width, int height, int flags)
{
	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (m_window != 0)
	{
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		if (m_renderer != 0)
		{
			SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
			m_camera = camera;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Renderer::cleanUp()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
}

void Renderer::clear()
{
	SDL_RenderClear(m_renderer);
}

void Renderer::present()
{
	setDrawColour(0, 0, 0, 255);
	SDL_RenderPresent(m_renderer);
}

void Renderer::setDrawColour(int r, int g, int b, int a) const
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

SDL_Renderer* Renderer::getSDLRenderer()
{
	return m_renderer;
}

void Renderer::setDrawColour(const Colour& c) const
{
	SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
}

void Renderer::drawRect(SDL_Rect r, const Colour& colour) const
{
	//r = applyCameraTransformation(r);

	m_camera->worldToScreen(r);
	
	setDrawColour(colour);
	SDL_RenderFillRect(m_renderer, &r);
	setDrawColour(0,0,0,255);
	SDL_RenderDrawRect(m_renderer, &r);
}
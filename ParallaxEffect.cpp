#include "ParallaxEffect.h"

Camera2D::ParallaxEffect::ParallaxEffect(bool scrollX)
	: Effect(Type::Parallax) 
	, m_scrollX(scrollX)
{
}

void Camera2D::ParallaxEffect::init(SDL_Renderer * renderer, const SDL_Rect& bounds)
{
	for (Layer& l : m_layers)
	{
		l.init(m_scrollX, renderer, bounds);
	}
}

void Camera2D::ParallaxEffect::addLayer(const Layer & layer)
{
	m_layers.push_back(layer);
}

void Camera2D::ParallaxEffect::draw(SDL_Renderer * renderer)
{
	for (Layer& l : m_layers)
	{
		l.draw(renderer);
	}
}

void Camera2D::ParallaxEffect::update(float vel, const SDL_Rect& bounds)
{
	for (Layer& l : m_layers)
	{
		l.update(vel, bounds);
	}
}


bool Camera2D::ParallaxEffect::getScrollX() const
{
	return m_scrollX;
}

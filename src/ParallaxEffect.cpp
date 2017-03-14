#include "ParallaxEffect.h"

Camera2D::ParallaxEffect::ParallaxEffect(bool scrollX)
	: Effect(Type::Parallax) 
	, m_scrollX(scrollX)
{
}

void Camera2D::ParallaxEffect::init(SDL_Renderer * renderer, const SDL_Rect& bounds)
{
	for (auto& layer : m_layers)
	{
		layer.second.init(m_scrollX, renderer, bounds);
	}
}

void Camera2D::ParallaxEffect::addLayer(const std::string& name, const Layer & layer)
{
	m_layers.insert(std::pair<std::string, Layer>(name, layer));
}

void Camera2D::ParallaxEffect::draw(SDL_Renderer * renderer)
{
	for (auto& layer : m_layers)
	{
		layer.second.draw(renderer);
	}
}

void Camera2D::ParallaxEffect::update(const Vector2& vel, const SDL_Rect& bounds, const Vector2& shakeOffset)
{
	for (auto& layer : m_layers)
	{
		layer.second.update(vel, bounds, shakeOffset);
	}
}


bool Camera2D::ParallaxEffect::getScrollX() const
{
	return m_scrollX;
}

int Camera2D::ParallaxEffect::getLayersSize() const
{
	return m_layers.size();
}

Camera2D::Layer* Camera2D::ParallaxEffect::getLayer(const std::string & name)
{
	if (m_layers.find(name) != m_layers.end())
	{
		return &m_layers.at(name);
	}
	else
	{
		return nullptr;
	}
}


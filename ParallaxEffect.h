#pragma once
#include <unordered_map>
#include "Effect.h"
#include "Layer.h"

namespace Camera2D
{
	class ParallaxEffect : public Effect
	{
	public:
		ParallaxEffect(bool scrollX = true);
		void init(SDL_Renderer* renderer, const SDL_Rect& bounds);
		void addLayer(const std::string& name, const Layer& layer);
		void draw(SDL_Renderer* renderer);
		void update(const Vector2& vel, const SDL_Rect& bounds, const Vector2& shakeOffset = Vector2());
		bool getScrollX() const;
		int getLayersSize() const;
		Layer* getLayer(const std::string& name);
	private:
		bool m_scrollX;
		std::unordered_map<std::string, Layer> m_layers;
	};
}
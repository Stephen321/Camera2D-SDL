#pragma once
#include "Effect.h"
#include "Layer.h"

namespace Camera2D
{
	class ParallaxEffect : public Effect
	{
	public:
		ParallaxEffect() : Effect(Type::Parallax) {};
		ParallaxEffect(bool scrollX);
		void init(SDL_Renderer* renderer, const SDL_Rect& bounds);
		void addLayer(const Layer& layer);
		void draw(SDL_Renderer* renderer);
		void update(float vel, const SDL_Rect& bounds);
		bool getScrollX() const;
	private:
		bool m_scrollX;
		std::vector<Layer> m_layers;
	};
}
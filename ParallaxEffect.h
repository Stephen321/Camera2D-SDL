#pragma once
#include "Effect.h"
#include "Layer.h"

namespace Camera2D
{
	class ParallaxEffect : public Effect
	{
	public:
		ParallaxEffect() {};
		ParallaxEffect(bool scrollX);
		void init(SDL_Renderer* renderer, const SDL_Rect& bounds);
		void addLayer(const Layer& layer);
		void setName(const std::string& name);
		void draw(SDL_Renderer* renderer);
		void update(float vel, const SDL_Rect& bounds);
		std::string getName() const;
		bool getScrollX() const;
	private:
		bool m_scrollX;
		std::string m_name;
		std::vector<Layer> m_layers;
	};
}
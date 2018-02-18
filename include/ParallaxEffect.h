#ifndef PARALLAXEFFECT_H
#define PARALLAXEFFECT_H

#include <unordered_map>
#include "Effect.h"
#include "Layer.h"

namespace Camera2D
{
	class CAMERA2D_API ParallaxEffect : public Effect
	{
	public:
        //ParallaxEffect always needs a SDL renderer which it will draw to
		ParallaxEffect(SDL_Renderer* renderer, bool scrollX = true);

		void init(const SDL_Rect& bounds);
		void addLayer(const std::string& name, const Layer& layer);
		void draw();
		void update(const Vector2& vel, const SDL_Rect& bounds, const Vector2& shakeOffset = Vector2());
		bool getScrollX() const;
		int getLayersSize() const;
		Layer* getLayer(const std::string& name);
	private:
		bool m_scrollX;
		std::unordered_map<std::string, Layer> m_layers;
        SDL_Renderer* m_renderer;
	};
}

#endif
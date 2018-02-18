#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <string>
#include "SDL2/SDL.h"
#include "Vector2.h"
#include "Config.h"

namespace Camera2D
{
	class CAMERA2D_API Layer
	{
	public:
		Layer(const std::string& path, float scrollMultiplier, int sections = 3, bool locked = true);
		void init(bool scrollX, SDL_Renderer* renderer, const SDL_Rect& bounds);
		void draw(SDL_Renderer* target) const;
		void update(const Vector2& vel, const SDL_Rect& bounds, const Vector2& shakeOffset);
		Layer& operator=(const Layer& rhs) { return *this; };
		~Layer();
		void setScrollMultiplier(float scrollMultiplier);
		float getScrollMultiplier() const;
	private:
		Vector2 m_shakeOffset;
		int clamp(int value, int min, int max);
		virtual void positionSection(const SDL_Rect& bounds, int section, int xDir, int yDir = 0);
		SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);

		bool m_scrollX;
		bool m_locked; //if locked false then the axis we arent scrolling on can be moved 
		std::string m_path;
		const int SECTIONS;
		struct Section
		{
			SDL_Texture* texture;
			SDL_Rect dest;
			float x;
			float y;
		};
		int m_textureSections;
		std::vector<Section> m_sections;
		int m_left, m_middle, m_right;
		float m_scrollMultiplier;
	};
}

#endif
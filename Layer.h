#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include <iostream>
namespace Camera2D
{
	class Layer
	{
	public:
		Layer(const std::string& path, float scrollMultiplier, int sections = 3);
		void init(bool scrollX, SDL_Renderer* renderer, const SDL_Rect& bounds);
		void draw(SDL_Renderer* target) const;
		void update(float vel, const SDL_Rect& bounds);
		Layer& operator=(const Layer& rhs) { return *this; };
		~Layer();
	private:
		int clamp(int value, int min, int max);
		virtual void positionSection(const SDL_Rect& bounds, int section, int xDir, int yDir = 0);
		SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);

		bool m_scrollX;
		std::string m_path;
		const int SECTIONS;
		struct Section
		{
			SDL_Texture* texture;
			SDL_Rect dest;
			float x;
			float y;
		};
		std::vector<Section> m_sections;
		int m_left, m_middle, m_right;
		float m_scrollMultiplier;
	};
}
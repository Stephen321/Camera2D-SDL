#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <string>
#include "Config.h"
#include "SDL2/SDL.h"
#include "Vector2.h"

namespace Camera2D
{
	/*! \brief A layer to be used with the parallax effect.
	*/
	class CAMERA2D_API Layer
	{
	public:
		/*! \brief
		\param path Path of the textures to load.
		\param scrollMultiplier How fast to scroll.
		\param sections How many different textures there are.
		\param locked If locked to one axis.
		*/
		Layer(const char* path, float scrollMultiplier, int sections = 3, bool locked = true);

		/*! \brief Initialise the layer.
		\param scrollX Should the parallax be on the x or y axis.
		\param renderer The renderer used to draw the parallax textures to.
		\param bounds Bounds of the camera.
		*/
		void init(bool scrollX, SDL_Renderer* renderer, const SDL_Rect& bounds);

		/*! \brief Draw the parallax effect to a SDL_Renderer.
		\param target The renderer to draw to.
		*/
		void draw(SDL_Renderer* target) const;

		/*! \brief Update the parallax effect.
		\param vel Velocity of the camera.
		\param bounds Bounds of the camera.
		\param shakeOffset How much shake there is.
		*/
		void update(const Vector2& vel, const SDL_Rect& bounds, const Vector2& shakeOffset);
		Layer& operator=(const Layer& rhs) { return *this; };
		~Layer();

		/*! \brief Set the scroll multiplier.
		\param scrollMultiplier
		*/
		void setScrollMultiplier(float scrollMultiplier);

		/*! \brief Get the scroll multiplier.
		\return float
		*/
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
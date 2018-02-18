#ifndef PARALLAXEFFECT_H
#define PARALLAXEFFECT_H

#include <unordered_map>
#include "Effect.h"
#include "Layer.h"

namespace Camera2D
{
	/*! \brief An effect which emulates parallax scrolling.
	*/
	class CAMERA2D_API ParallaxEffect : public Effect
	{
	public:

		/*! \brief Parallax effect always requires an SDL_Renderer to be created.
		\param renderer The renderer used in the creation of the parallax effect.
		\param scrollX Which axis to scroll on.
		*/
		ParallaxEffect(SDL_Renderer* renderer, bool scrollX = true);

		/*! \brief Initialise the parallax effect.
		\param bounds The bounds of the camera.
		*/
		void init(const SDL_Rect& bounds);

		/*! \brief Add a layer to the parallax effect.
		\param name The name of the new layer.
		\param layer The layer to add.
		*/
		void addLayer(const std::string& name, const Layer& layer);

		/*! \brief Draw the parallax effect.
		*/
		void draw();

		/*! \brief Update the parallax effect.
		\param vel Velocity of the camera.
		\param bounds Bounds of the camera.
		\param shakeOffset How much shake there is.
		*/
		void update(const Vector2& vel, const SDL_Rect& bounds, const Vector2& shakeOffset = Vector2());

		/*! \brief Get if the parallax effect is scrolling on the x or y axis.
		\return bool
		*/
		bool getScrollX() const;

		/*! \brief Get how many layers there.
		\return int
		*/
		int getLayersSize() const;

		/*! \brief Get a layer by name
		\param name The name of the layer get.
		\return Layer*
		*/
		Layer* getLayer(const std::string& name);
	private:
		bool m_scrollX;
		std::unordered_map<std::string, Layer> m_layers;
        SDL_Renderer* m_renderer;
	};
}

#endif
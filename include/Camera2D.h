#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <vector>
#include "SDL2\SDL.h"
#include "ParallaxEffect.h"
#include "ShakeEffect.h"
#include "Attractor.h"
#include "Repulsor.h"


/*! \brief Contains everything to do with the Camera2D.
*/
namespace Camera2D
{
	const float DEFAULT_ACCEL = 100.f; //every second how much you change velocity by
	const float DEFAULT_MAX_VEL = 200.f; //how fast you move in a second
	const float DEFAULT_DRAG = 0.9f;
	const float MIN_VEL = 5.f;

	const float DEFAULT_ZOOM_SPEED = 0.01f;
	const float DEFAULT_ZOOMTO_SPEED = 1.f;
	const float DEFAULT_MIN_ZOOM = 1.5f;
	const float DEFAULT_MAX_ZOOM = 0.5f;


	/*! \brief The class used to do everything with the camera.

	Contains all the functions for manipulating the camera.
	This will consist of a camera of specified width and height being created.
	Only objects within the camera bounds will be visible. The camera will fit the
	window of the application. It will be capable of a number of basic transformations
	(pan, zoom) and more advanced functionality (parallax). It should be easy for a
	user to integrate into any game whether it's side on such as in a platformer or
	if it's top down.
	Different functions modify the zoom/position of the camera of the camera in
	different ways. These values are used to compute a screen point/rectangle
	from a given "world" point/rectangle. This transformation allows the new
	rectangle to be drawn in such a way that it appears as if there is a camera.
	(In reality, everything else is being displaced.)
	*/
	class CAMERA2D_API Camera
	{
	public:
		/*! \brief Initialises class member variables.
		*/
		Camera();

		/*! \brief Initilise the camera with the window size.
		\param windowWidth Width of the window.
		\param windowHeight Height of the window.
		*/
		void init(int windowWidth, int windowHeight);

		/*! \brief Set the centre of the camera to x and y coordinates.
		\param x X position to set the centre to.
		\param y Y position to set the centre to.
		*/
		void setCentre(float x, float y);

		/*! \brief Set the centre of the camera to a point.
		\see Point
		\param p Point to set the centre to.
		*/
		void setCentre(const Point& p);

		/*! \brief Set the size of the camera.
		\param width Width to set the camera to.
		\param height Height to set the camera to.
		*/
		void setSize(int width, int height);

		/*! \brief Get the centre of the camera.
		\return Vector2
		*/
		Vector2 getCentre() const;

		/*! \brief Get the size of the camera.
		\return Vector2
		*/
		Vector2 getSize() const;

		/*! \brief Get the bounds of the camera.
		\return SDL_Rect
		*/
		SDL_Rect getBounds() const;

		/*! \brief Convert a rectangle from world coordinates to screen coordinates.
		\param r A rectangle in world coordinates.
		\return SDL_Rect
		*/
		SDL_Rect worldToScreen(const SDL_Rect& r) const;

		/*! \brief Convert a point from world coordinates to screen coordinates.
		\param p A point in world coordinates.
		\return Point
		*/
		Point worldToScreen(const Point& p) const;


		/*! \brief Convert a rectangle from screen coordinates to world coordinates.
		\param sr A rectangle in screen corrdinates.
		\return SDL_Rect
		*/
		SDL_Rect screenToWorld(const SDL_Rect& sr) const;

		/*! \brief Convert a point from screen coordinates to world coordinates.
		\param sp A point in screen coordinates.
		\return Point
		*/
		Point screenToWorld(const Point& sp) const;

		/*! \brief Checks if a rectangle intersects with the camera.
		\param r A rectangle in screen coordinates.
		\return bool
		*/
		bool intersects(const SDL_Rect& r) const; 

		/*! \brief Checks if a point intersects with the camera
		\param p A point in screen coordinates.
		\return bool
		*/
		bool intersects(const Point& p) const; 

		/*! \brief Set the motion properties of the camera. If not provided then defaulted.
		\param accelerationRate How fast the camera movement accelerates.
		\param maxVelocity Maximum velocity of the camera.
		\param drag How much drag effects the camera.
		*/
		void setMotionProps(float accelerationRate = DEFAULT_ACCEL, float maxVelocity = DEFAULT_MAX_VEL, float drag = DEFAULT_DRAG);

		/*! \brief Set the acceleration rate.
		\param accelerationRate How fast the camera movement accelerates.
		*/
		void setAccelerationRate(float accelerationRate);

		/*! \brief Get the acceleration rate of the camera.
		\return float
		*/
		float getAccelerationRate() const;

		/*! \brief Set the maximum velocity.
		\param maxVelocity Maximum velocity of the camera.
		*/
		void setMaxVelocity(float maxVelocity);
		float getMaxVelocity() const;

		/*! \brief Set the zoom properties of the camera. If not provided then defaulted.
		\see zoomTo()
		\param zoomSpeed Speed the camera zooms in or out.
		\param zoomToSpeed When using zoomTo function.
		\param minZoom Minimum level of zoom.
		\param maxZoom Maximum level of zoom.
		*/
		void setZoomProps(float zoomSpeed = DEFAULT_ZOOM_SPEED, float zoomToSpeed = DEFAULT_ZOOMTO_SPEED, float minZoom = DEFAULT_MIN_ZOOM,
						  float maxZoom = DEFAULT_MAX_ZOOM);

		/*! \brief Set the min and max of camera zoom. Defaulted to unlimited.
		\param min Minimum level of zoom.
		\param max Maximum level of zoom.
		*/
		void setZoomMinMax(float min = -1, float max = -1); //default to unlimited

		/*! \brief Enable panning on the x and y directions.
		\param xDir Pan horizontally. Negative 1 for left, positive 1 for right, 0 for no movement.
		\param yDir Pan vertically. Negative 1 for up, positive 1 for down, 0 for no movement.
		*/
		void pan(int xDir, int yDir);

		/*! \brief Enable panning on the x axis.
		\param xDir Pan horizontally. Negative 1 for left, positive 1 for right, 0 for no movement.
		*/
		void panX(int xDir);

		/*! \brief Enable panning on the y axis.
		\param yDir Pan vertically. Negative 1 for up, positive 1 for down, 0 for no movement.
		*/
		void panY(int yDir);

		/*! \brief Enable zooming in or out.
		\param zoomDir Zoom in or out. Negative 1 for out, positive 1 for in.
		*/
		void zoom(int zoomDir);

		/*! \brief Zoom to a certain zoom level over time.
		\param target The value to zoom to.
		\param time Time to spend zooming to the target zoom value.
		*/
		void zoomTo(float target, float time = -1.f);

		/*! \brief Zoom to a certain zoom level on both x and y axis over time. Changes aspect ratio.
		\param target Seperate zooms for x and y.
		\param time Time to spend zooming to the target zoom value.
		*/
		void zoomTo(const Vector2& target, float time = -1.f);

		/*! \brief Zoom and move the camera to contain a vector of points over time.
		\param points Vector of points.
		\param keepZoomRatio Should the aspect ratio be allowed to change.
		\param time Time to spend zooming to the target value and position.
		*/
		void zoomToFit(const std::vector<Point>& points, bool keepZoomRatio = true, float time = -1.f);
		
		/*! \brief Zoom and move the camera to contain a vector of rectangles over time.
		\param rects Vector of rectangles.
		\param keepZoomRatio Should the aspect ratio be allowed to change.
		\param time Time to spend zooming to the target value and position.
		*/
		void zoomToFit(const std::vector<SDL_Rect>& rects, bool keepZoomRatio = true, float time = -1.f);
		
		/*! \brief Reset the zoom ratio if it has changed.
		*/
		void resetZoomRatio();

		/*! \brief Zoom to a certain level.
		\param zoomAmount Zoom amount to zoom to.
		*/
		void setZoom(float zoomAmount);

		/*! \brief Get the maximum zoom.
		\return float
		*/
		float getZoomMax() const;

		/*! \brief Get the minimum zoom.
		\return float
		*/
		float getZoomMin() const;

		/*! \brief Update the camera motions and zooms.
		\param deltaTime Time passed between frames to allow frame indepentant motion.
		*/
		void update(float deltaTime);

		/*! \brief Draw the parallax effects.
		\see ParallaxEffect
		*/
		void drawParallax();

		/*! \brief Move the camera by certain amounts.
		\param x Amount to move on the x axis.
		\param y Amount to move on the y axis.
		*/
		void moveBy(float x, float y);

		/*! \brief Allow movement on the horizontal axis.
		\param value Allow movement on the horizontal axis.
		*/
		void setAllowedHorizontal(bool value);

		/*! \brief Get if movement is allowed on the horizontal axis
		\return bool
		*/
		bool getAllowedHorizontal() const;

		/*! \brief Allow movement on the vertical axis.
		\param value Allow movement on the vertical axis.
		*/
		void setAllowedVertical(bool value);
		
		/*! \brief Get if movement is allowed on the vertical axis
		\return bool
		*/
		bool getAllowedVertical() const;


		/*! \brief Stop motion.
		*/
		void lockMotion();


		/*! \brief Allow motion.
		*/
		void unlockMotion();


		/*! \brief Add a new effect to the camera.
		\param effect The effect to add.
		\param name Name of the effect.
		*/
		void addEffect(Effect& effect, const std::string& name = "");


		/*! \brief Remove an effect by name.
		\param name Name of the effect to remove.
		*/
		void removeEffect(const std::string& name);


		/*! \brief Remove first effect added of a certain type.
		\param type The type of effect to remove.
		*/
		void removeEffect(Effect::Type type);	

		/*! \brief Start an effect by type.
		\param type The type of effect to start.
		*/
		void startEffect(Effect::Type type);

		/*! \brief Start an effect by name.
		\param name The name of the effect to start.
		*/
		void startEffect(const std::string& name);

		/*! \brief End an effect by type and optionally remove it.
		\param type The type of the effect to end.
		\param remove Should the effect also be removed.
		*/
		void endEffect(Effect::Type type, bool remove = false);

		/*! \brief End an effect by name and optionally remove it.
		\param name The name of the effect to end.
		\param remove Should the effect also be removed
		*/
		void endEffect(const std::string& name, bool remove = false);

		/*! \brief End all effects.
		*/
		void endEffects();

		/*! \brief Find a parallax effect.
		\see ParallaxEffect
		\param name Name of the parallax effect to find.
		\return ParallaxEffect*
		*/
		ParallaxEffect* findParallax(const std::string& name);

		/*! \brief Find a shake effect.
		\param name Name of the shake effect to find.
		\return ShakeEffect*
		*/
		ShakeEffect* findShake(const std::string& name);

		/*! \brief Find an effect.
		\param name Name of the effect to find.
		\return Effect*
		*/
		Effect * findEffect(const std::string & name);

		/*! \brief Find an influencer by name.
		\param name Name of the influencer to find.
		\return Influencer*
		*/
		Influencer* findInfluencer(const std::string& name);

		/*! \brief Reset acceleration and velocity.
		*/
		void resetMotion();

		/*! \brief Restrict movement of the centre of the camera to a certain rectangle area.
		\param restrictBounds The rectangle area to restrict to.
		*/
		void restrictCentre(SDL_Rect restrictBounds);

		/*! \brief Stop the centre being restricted.
		*/
		void unrestrictCentre();

		/*! \brief Add an influencer to the camera.
		\param influencer The influencer to add.
		\param name Name of the influencer.
		*/
		void addInfluencer(Influencer& influencer, const std::string& name = "");

		/*! \brief Remove an influencer by name.
		\param name Name of the influencer to remove.
		*/
		void removeInfluencer(const std::string& name);

		/*! \brief Remove all influencers.
		\param type Type of the influencers to remove.
		*/
		void removeAllInfluencer(Influencer::Type type);

		/*! \brief Set the stop velocity of the attractor.
		\param attractorStopVel The value to set the stop velocity of the attractor to.
		*/
		void setAttractorStopVel(float attractorStopVel);

		/*! \brief Get the attractor stop velocity.
		\return float
		*/
		float getAttractorStopVel() const;

	private:
		const float REPULSOR_WEIGHT = 0.6f;
		void updateMotion(float deltaTime);
		void updateZoom(float deltaTime);
		void updateEffects(float deltaTime);
		Vector2 lerp(const Vector2& v1, const Vector2& v2, float percent);
		float lerp(float start, float end, float percent);
		void changeBoundsZoom();
		float clampZoom(float num);
		Vector2 getInfluencorAccel();
		Vector2 getAttractorsAccel();
		Vector2 getRepulsorsAccel();

		float m_attractorStopVel;

		//effects
		std::vector<ParallaxEffect> m_parallaxEffects;
		ParallaxEffect* m_currentParallax;
		std::vector<ShakeEffect> m_shakeEffects;
		ShakeEffect* m_currentShake;

		//attractor/repulsor
		std::vector<Attractor> m_attractors;
		const float ATTRACTOR_ARIVE_RADIUS = 100.f;
		const float ATTRACTOR_ARRIVED_RADIUS = 15.f;
		const float ATTRACTOR_TIME_TO_ARRIVE = 0.5f;
		const float DEFAULT_ATTRACTOR_STOP_VEL = 20.f;
		std::vector<Repulsor> m_repulsors;


		SDL_Rect m_bounds;

		int m_windowWidth;
		int m_windowHeight;

		//motion props
		float m_accelerationRate;
		float m_maxVelocity;
		float m_drag;
		Vector2 m_velocity;
		Vector2 m_direction;
		Vector2 m_centre;
		Vector2 m_acceleration;
		float m_timeSinceLastXAccel;
		float m_timeSinceLastYAccel;
		const float MAX_TIME_BEFORE_ACCEL_RESET = 0.5f;

		bool m_allowedHorizontal;
		bool m_allowedVertical;

		//zoom props
		Vector2 m_zoom;
		Vector2 m_zoomDirection;
		float m_zoomSpeed;
		float m_zoomToSpeed;
		float m_minZoom; //how far out we can zoom
		float m_maxZoom; //how far in to zoom

		//lerp centre
		bool m_zoomToFitActive;
		Vector2 m_zoomToFitStart;
		Vector2 m_zoomToFitTarget;
		float m_zoomToFitTime;
		float m_zoomToFitMaxTime;

		//lerp zoom
		bool m_zoomToActive;
		Vector2 m_zoomStart;
		Vector2 m_zoomTarget;
		float m_zoomToTime;
		float m_zoomToMaxTime;

		bool m_ratioResetting;

		bool m_updating;

		bool m_restrictCentre;
		SDL_Rect m_restrictBounds;
	};
}

#endif
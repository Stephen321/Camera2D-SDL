#pragma once

#include "SDL.h"
#include <vector>
#include "ParallaxEffect.h"
#include "ShakeEffect.h"
#include "Attractor.h"
#include "Repulsor.h"

using namespace std;

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


	class Camera
	{
	public:
		Camera();
		void init(int windowWidth, int windowHeight, SDL_Renderer* renderer);
		void setCentre(float x, float y);
		void setCentre(const Point& p);
		void setSize(int width, int height);
		Vector2 getCentre() const;
		Vector2 getSize() const;
		SDL_Rect getBounds() const;
		SDL_Rect worldToScreen(const SDL_Rect& r) const;
		Point worldToScreen(const Point& p) const;

		SDL_Rect screenToWorld(const SDL_Rect& sr) const;
		Point screenToWorld(const Point& sp) const;

		bool intersects(const SDL_Rect& r) const; //check if world rect is within camera
		bool intersects(const Point& p) const; //check if world point is within camera

		void setMotionProps(float accelerationRate = DEFAULT_ACCEL, float maxVelocity = DEFAULT_MAX_VEL, float drag = DEFAULT_DRAG);

		void setAccelerationRate(float accelerationRate);
		float getAccelerationRate() const;
		void setMaxVelocity(float maxVelocity);
		float getMaxVelocity() const;

		void setZoomProps(float zoomSpeed = DEFAULT_ZOOM_SPEED, float zoomToSpeed = DEFAULT_ZOOMTO_SPEED, float minZoom = DEFAULT_MIN_ZOOM,
						  float maxZoom = DEFAULT_MAX_ZOOM);
		void setZoomMinMax(float min = -1, float max = -1); //default to unlimited
		void pan(int xDir, int yDir);
		void panX(int xDir);
		void panY(int yDir);
		void zoom(int zoomDir);
		void zoomTo(float target); 
		void zoomTo(const Vector2& target);
		void zoomTo(float targetX, float targetY);
		void zoomToFit(const std::vector<Point>& points, bool keepZoomRatio = true); 
		void zoomToFit(const std::vector<SDL_Rect>& rects, bool keepZoomRatio = true);
		void resetZoomRatio();
		void update(float deltaTime);
		void render();
		void moveBy(float x, float y);

		void setAllowedHorizontal(bool value);
		bool getAllowedHorizontal() const;
		void setAllowedVertical(bool value);
		bool getAllowedVertical() const;
		void lockMotion();
		void unlockMotion();

		//effects
		void addEffect(Effect& effect, const std::string& name = "");
		void removeEffect(const std::string& name);
		void removeEffect(Effect::Type type);		
		void startEffect(Effect::Type type);
		void startEffect(const std::string& name);
		void endEffect(Effect::Type type, bool remove = false);
		void endEffect(const std::string& name, bool remove = false);
		void endEffects();

		ParallaxEffect* findParallax(const std::string& name);
		ShakeEffect* findShake(const std::string& name);
		Effect * findEffect(const std::string & name);

		Influencer* findInfluencer(const std::string& name);

		void resetMotion();

		//Influencers
		void addInfluencer(Influencer& influencer, const std::string& name = "");
		void removeInfluencer(const std::string& name);
		void removeAllInfluencer(Influencer::Type type);

		void setAttractorStopVel(float attractorStopVel);
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
		SDL_Renderer* m_renderer;

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
	};
}
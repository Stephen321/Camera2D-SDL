#pragma once

#include "SDL.h"
#include <vector>
#include <numeric>
#include <iostream>
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
		void setZoomProps(float zoomSpeed = DEFAULT_ZOOM_SPEED, float zoomToSpeed = DEFAULT_ZOOMTO_SPEED, float minZoom = DEFAULT_MIN_ZOOM,
						  float maxZoom = DEFAULT_MAX_ZOOM);
		void setZoomMinMax(float min = -1, float max = -1); //default to unlimited
		void pan(int xDir, int yDir);
		void panX(int xDir);
		void panY(int yDir);
		void zoom(int zoomDir);
		void zoomTo(float target); //TODO: big gaps between rects...
		void zoomTo(const Vector2& target);
		void zoomTo(float targetX, float targetY);
		void zoomToFit(const std::vector<Point>& points, bool keepZoomRatio = true); //TODO: okay to use vector?
		void zoomToFit(const std::vector<SDL_Rect>& rects, bool keepZoomRatio = true);
		void resetZoomRatio();
		void update(float deltaTime);
		void render();
		void moveBy(float x, float y);

		void allowedHorizontal(bool value);
		void allowedVertical(bool value);
		void setLock(bool value);

		//effects
		void addEffect(Effect& effect, const std::string& name = "");
		void removeEffect(const std::string& name);
		void removeEffect(Effect::Type type);		
		void startEffect(Effect::Type type);
		void startEffect(const std::string& name);
		void endEffect(Effect::Type type, bool remove = false);
		void endEffect(const std::string& name, bool remove = false);
		void endEffects();

		Effect* findEffect(const std::string& name);
		Affector* findAffector(const std::string& name);

		void stopMotion();

		//edge snapping
		void enableEdgeSnap();
		void setEdgeSnapIntervals(int xInterval, int yInterval, float intervalTime, bool enable = true);
		void disableEdgeSnap();

		//affectors
		void addAffector(Affector& affector, const std::string& name = "");
		void removeAffector(const std::string& name);

	private:
		void updateMotion(float deltaTime);
		void updateZoom(float deltaTime);
		void updateEffects(float deltaTime);
		Vector2 lerp(const Vector2& v1, const Vector2& v2, float percent);
		float lerp(float start, float end, float percent);
		void changeBoundsZoom();
		float clampZoom(float num);
		Vector2 getAffectorAccel();
		Vector2 getAttractorsAccel();
		Vector2 getRepulsorsAccel();

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
		const float ATTRACTOR_WEIGHT = 0.001f;
		const float ATTRACTOR_STOP_VEL = 20.f;
		std::vector<Repulsor> m_repulsors;
		const float REPULSOR_WEIGHT = 0.01f;


		SDL_Rect m_bounds;
		SDL_Renderer* m_renderer;

		int m_windowWidth;
		int m_windowHeight;

		//edge snapping
		bool m_edgeSnapping;
		int m_xEdgeInterval;
		float m_xStart;
		int m_yEdgeInterval;
		float m_intervalTime;
		float m_intervalElapsedTime;

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

		//TODO: should we use sdl image to render other formats than bmp??

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
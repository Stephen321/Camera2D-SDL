#pragma once

#include "SDL.h"
#include <vector> //use array instead?
#include <iostream>

using namespace std;

namespace Camera2D
{
	typedef struct Vector2 {
		Vector2(float _x, float _y)
			: x(_x), y(_y) {};
		Vector2()
			: x(0), y(0) {};
		Vector2 operator*(float s) {
			Vector2 v(x, y);
			v.x *= s;
			v.y *= s;
			return v;
		};
		Vector2& operator+=(const Vector2& v) {
			x += v.x;
			y += v.y;
			return *this;
		};
		Vector2 operator+(const Vector2& rhs) const {
			Vector2 v(x, y);
			v += rhs;
			return v;
		};
		Vector2& operator/=(float s) {
			x /= s;
			y /= s;
			return *this;
		};
		Vector2& operator/(float s) const {
			Vector2 v(x, y);
			v /= s;
			return v;
		};
		Vector2& operator-=(const Vector2& v) {
			x -= v.x;
			y -= v.y;
			return *this;
		};
		Vector2 operator-(const Vector2& rhs) const {
			Vector2 v(x, y);
			v -= rhs;
			return v;
		};
		bool operator==(const Vector2& v) {
			return (x == v.x && y == v.y);
		};
		Vector2 normalize() const {
			if (length() == 0.f)
				return Vector2();
			Vector2 v(x, y);
			v.x /= length();
			v.y /= length();
			return v;
		};
		float length() const {
			return sqrt(x* x + y * y);
		};
		 void limit(float max) {
			if (length() > max)
			{
				*this = normalize() * max;
			}
		};
		float x;
		float y;
	} Size, Point;

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
		Vector2 getPosition() const;
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
		void resetZoomRatio(bool resetToX = true);
		void update(float deltaTime);
		void render();
		void moveBy(float x, float y);

	private:
		void updateMotion(float deltaTime);
		void updateZoom(float deltaTime);
		Vector2 lerp(const Vector2& v1, const Vector2& v2, float percent);
		float lerp(float start, float end, float percent);
		void changeBoundsZoom();
		float clampZoom(float num);

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

		//TODO: should we use sdl image to render other formats than bmp??

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
#pragma once

#include "SDL.h"
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
		Vector2& operator-=(const Vector2& v) {
			x -= v.x;
			y -= v.y;
			return *this;
		};
		Vector2 normalize() {
			if (length() == 0.f)
				return Vector2();
			Vector2 v(x, y);
			v.x /= length();
			v.y /= length();
			return v;
		};
		float length() {
			return sqrt(x* x + y * y);
		}
		float x;
		float y;
	} Size, Point;

	const float DEFAULT_ACCEL = 100.f; //every second how much you change velocity by
	const float DEFAULT_MAX_VEL = 200.f; //how fast you move in a second
	const float DEFAULT_DRAG = 0.9f;
	const float MIN_VEL = 5.f;

	const float DEFAULT_ZOOM_SPEED = 0.01f;
	const float DEFAULT_ZOOMTO_SPEED = 0.0001f;
	const float DEFAULT_MIN_ZOOM = 1.5f;
	const float DEFAULT_MAX_ZOOM = 0.5f; 
	const float DEFAULT_ZOOM_SNAP = 100.f;

	class Camera
	{
	public:
		Camera();
		void init(int windowWidth, int windowHeight, SDL_Renderer* renderer);
		void setPosition(float x, float y);
		void setPosition(const Point& p);

		bool worldToScreen(SDL_Rect& r) const;
		bool worldToScreen(Point& p) const;

		SDL_Rect screenToWorld(const SDL_Rect& sr) const;
		Point screenToWorld(const Point& sp) const;

		void setMotionProps(float accelerationRate = DEFAULT_ACCEL, float maxVelocity = DEFAULT_MAX_VEL, float drag = DEFAULT_DRAG);
		void setZoomProps(float zoomSpeed = DEFAULT_ZOOM_SPEED, float zoomToSpeed = DEFAULT_ZOOMTO_SPEED, float minZoom = DEFAULT_MIN_ZOOM,
						  float maxZoom = DEFAULT_MAX_ZOOM, float snapBackSpeed = DEFAULT_ZOOM_SNAP);
		void setZoomMinMax(float min = -1, float max = -1); //default to unlimited
		void pan(int xDir, int yDir);
		void panX(int xDir);
		void panY(int yDir);
		void zoom(int zoomDir);
		void zoomTo(float target);
		void update(float deltaTime);
		void render();
		void moveBy(float x, float y);

	private:
		void updateMotion(float deltaTime);
		void updateZoom(float deltaTime);
		void calculateBounds();

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
		Vector2 m_position;

		//zoom props
		float m_zoom;
		int m_zoomDirection;
		float m_zoomSpeed;
		float m_zoomToSpeed;
		float m_minZoom; //how far out we can zoom
		float m_maxZoom; //how far in to zoom
		float m_snapBackSpeed;
		bool m_zoomToActive;
		float m_zoomTarget;
	};
}
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

	const static float DEFAULT_ACCEL = 100.f; //every second how much you change velocity by
	const static float DEFAULT_MAX_VEL = 200.f; //how fast you move in a second
	const static float DEFAULT_DRAG = 0.9f;
	const static float MIN_VEL = 5.f;

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
		void pan(int xDir, int yDir);
		void panX(int xDir);
		void panY(int yDir);
		void update(float deltaTime);
		void render();
		void moveBy(float x, float y);

	private:
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
	};
}
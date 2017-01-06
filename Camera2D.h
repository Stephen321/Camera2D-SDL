#pragma once

#include "SDL.h"
#include <iostream>

using namespace std;

namespace Camera2D
{
	typedef struct Vector2 {
		Vector2(int _x, int _y)
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
		int x;
		int y;
	} Size, Point;

	//struct Rectangle {
	//	Rectangle(int _x, int _y, int _width, int _height)
	//		: x(_x), y(_y), width(_width), height(_height) {};
	//	Rectangle()
	//		: x(0), y(0), width(0), height(0) {};
	//	int x;
	//	int y;
	//	int width;
	//	int height;
	//};

	const static float DEFAULT_ACCEL = 5.f;
	const static float DEFAULT_MAX_VEL = 10.f;
	const static float DEFAULT_DRAG = 0.6f;

	class Camera
	{
	public:
		Camera();
		void init(int windowWidth, int windowHeight, SDL_Renderer* renderer);
		void setPosition(float x, float y);
		void setPosition(const Point& p);

		bool worldToScreen(SDL_Rect& r) const;
		//SDL_Rect screenToWorld(const SDL_Rect& r);

		void setMotionProps(float accelerationRate = DEFAULT_ACCEL, float maxVelocity = DEFAULT_MAX_VEL, float drag = DEFAULT_DRAG);
		void pan(int xDir, int yDir);
		void update(float deltaTime);
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
#pragma once

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
		bool operator!=(const Vector2& v) {
			return (x != v.x || y != v.y);
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
}

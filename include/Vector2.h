#ifndef VECTOR2_H
#define VECTOR2_H

#include "Config.h"

namespace Camera2D
{
	/*! \brief 2D vector with x and y component and overloaded operators for common vector operations.
	*/
	typedef struct CAMERA2D_API Vector2 {
		/*! \brief Initialises the vector with a x and y value.
		\param _x X value of the vector.
		\param _y Y value of the vector.
		*/
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

		/*! \brief Return a normalized vector.
		\return Vector2
		*/
		Vector2 normalize() const {
			if (length() == 0.f)
				return Vector2();
			Vector2 v(x, y);
			v.x /= length();
			v.y /= length();
			return v;
		};

		/*! \brief Get the length of the vector.
		\return float
		*/
		float length() const {
			return sqrt(x* x + y * y);
		};

		/*! \brief Limit the vector to be below a certain value.
		\param max Maximum length to set the vector to.
		*/
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

#endif
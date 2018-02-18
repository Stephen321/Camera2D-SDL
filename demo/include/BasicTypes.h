#ifndef  BASICTYPES_H
#define BASICTYPES_H

typedef struct Colour {
	Colour(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
		: r(_r), g(_g), b(_b), a(_a) {}
	unsigned char r, g, b, a;

} Colour;


typedef struct Vector2f {
	Vector2f(float _x, float _y) : x(_x), y(_y) {}
	Vector2f() : x(0.f), y(0.f) {}
	float x;
	float y;
} Vector2f;

#endif 

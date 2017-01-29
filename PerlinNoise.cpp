#include "PerlinNoise.h"

double Camera2D::PerlinNoise::noise(double x, double y)
{
	double floorx = (double)((int)x);
	double floory = (double)((int)y);
	double s, t, u, v;
	s = findnoise(floorx, floory);
	t = findnoise(floorx + 1, floory);
	u = findnoise(floorx, floory + 1);
	v = findnoise(floorx + 1, floory + 1);

	double int1 = lerp(s, t, x - floorx);
	double int2 = lerp(u, v, x - floorx);
	return lerp(int1, int2, y - floory);
}
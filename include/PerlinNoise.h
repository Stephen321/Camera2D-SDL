#pragma once

//http://discuss.cocos2d-x.org/t/tutorial-cocos2d-camera-shake-effect-from-unity-updates/27215

namespace Camera2D
{
	class PerlinNoise
	{
	public:
		static double noise(double x, double y);
	private:
		static inline double findnoise(double x, double y)
		{
			int n = (int)x + (int)y * 57;
			n = (n << 13) ^ n;
			int nn = (n*(n*n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
			return 1.0 - ((double)nn / 1073741824.0);
		}

		static inline float lerp(float start, float end, float percent)
		{
			return start + ((end - start) * percent);
		}
	};
}

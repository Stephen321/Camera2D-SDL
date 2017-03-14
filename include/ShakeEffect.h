#pragma once
#include "Effect.h"
#include "Vector2.h"
#include "PerlinNoise.h"
#include "SDL.h"

namespace Camera2D
{
	const float SHAKE_DEFAULT_DURATION = 5.f;
	const float SHAKE_DEFAULT_SPEED = 2.f;
	const float SHAKE_DEFAULT_MAGNITUDE = 1.f;
	const float SHAKE_DEFAULT_RANGE = 1000.f;

	class ShakeEffect : public Effect
	{
	public:
		ShakeEffect() : Effect(Type::Shake) {};
		void init();
		void draw(SDL_Renderer* renderer);
		void update(float deltaTime);
		void setProps(float duration = SHAKE_DEFAULT_DURATION, float speed = SHAKE_DEFAULT_SPEED, float magnitude = SHAKE_DEFAULT_MAGNITUDE, float range = SHAKE_DEFAULT_RANGE);
		void setDuration(float duration);
		float getDuration() const;
		void setSpeed(float speed);
		float getSpeed() const;
		void setMagnitude(float magnitude);
		float getMagnitude() const;
		void setRange(float range);
		float getRange() const;
		Vector2 getShakeOffset() const;
		void setEnabled(bool enabled) override;
	private:

		inline float randomNumberF(float min, float max)
		{
			return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
		}
		inline float clamp(float value, float min, float max)
		{
			if (value < min)
			{
				return min;
			}
			if (value > max)
			{
				return max;
			}

			return value;
		}
		Vector2 m_shakeOffset;

		float m_elapsedTime;

		float m_duration;
		float m_speed;
		float m_magnitude;
		float m_startRange;
	};
}
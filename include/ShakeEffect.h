#ifndef SHAKEFFECT_H
#define SHAKEFFECT_H

#include "Effect.h"
#include "Vector2.h"

namespace Camera2D
{
	const float SHAKE_DEFAULT_DURATION = 5.f;
	const float SHAKE_DEFAULT_SPEED = 2.f;
	const float SHAKE_DEFAULT_MAGNITUDE = 1.f;
	const float SHAKE_DEFAULT_RANGE = 1000.f;

	/*! \brief An effect which emulates camera shake.
	*/
	class CAMERA2D_API ShakeEffect : public Effect
	{
	public:
		ShakeEffect() : Effect(Type::Shake) {};
		/*! \brief Initialises the shake effect.
		*/
		void init();

		/*! \brief Updates the shake effect.
		\param deltaTime How much time has passed between frames to make the movement frame independent.
		*/
		void update(float deltaTime);

		/*! \brief Set properties of the shake effect.
		\param duration Duration of the shake effect.
		\param speed Speed of the shake effect.
		\param magnitude Magnitude of the shake effect.
		\param range Range of the shake effect.
		*/
		void setProps(float duration = SHAKE_DEFAULT_DURATION, float speed = SHAKE_DEFAULT_SPEED, float magnitude = SHAKE_DEFAULT_MAGNITUDE, float range = SHAKE_DEFAULT_RANGE);

		/*! \brief Set the duration of the shake effect.
		\param duration Duration of the shake effect.
		*/
		void setDuration(float duration);

		/*! \brief Get the duration of the shake effect.
		\return float
		*/
		float getDuration() const;

		/*! \brief Set the speed of the shake effect.
		\param speed Speed of the shake effect.
		*/
		void setSpeed(float speed);

		/*! \brief Get the speed of the shake effect.
		\return float
		*/
		float getSpeed() const;

		/*! \brief Set the speed of the shake effect.
		\param magnitude Magnitude of the shake effect.
		*/
		void setMagnitude(float magnitude);

		/*! \brief Get the magnitude of the shake effect.
		\return float
		*/
		float getMagnitude() const;

		/*! \brief Set the range of the shake effect.
		\param range Range of the shake effect.
		*/
		void setRange(float range);

		/*! \brief Get the range of the shake effect.
		\return float
		*/
		float getRange() const;

		/*! \brief Get the shake offset.
		\return Vector2
		*/
		Vector2 getShakeOffset() const;

		/*! \brief Set if the shake effect is enabled.
		\param enabled Enabled value of the shake effect.
		*/
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

#endif
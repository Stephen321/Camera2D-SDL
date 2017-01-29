#include "ShakeEffect.h"

void Camera2D::ShakeEffect::init()
{
	setProps();
	m_elapsedTime = 0.f;
}

void Camera2D::ShakeEffect::draw(SDL_Renderer * renderer)
{
}

void Camera2D::ShakeEffect::update(float deltaTime)
{
	m_shakeOffset.x = 0.f;
	m_shakeOffset.y = 0.f;

	float randomStart = randomNumberF(-m_startRange, m_startRange); 
	m_elapsedTime += deltaTime;

	float percentComplete = m_elapsedTime / m_duration;

	// We want to reduce the shake from full power to 0 starting half way through
	float damper = 1.0f - clamp(2.0f * percentComplete - 1.0f, 0.0f, 1.0f);

	// Calculate the noise parameter starting randomly and going as fast as speed allows
	float alpha = randomStart + m_speed * percentComplete;

	// map noise to [-1, 1]
	float x = PerlinNoise::noise(alpha, 0.0f) * 2.0f - 1.0f;
	float y = PerlinNoise::noise(0.0f, alpha) * 2.0f - 1.0f;

	x *= m_magnitude * damper;
	y *= m_magnitude * damper;
	m_shakeOffset.x = x;
	m_shakeOffset.y = y;

	if (m_elapsedTime >= m_duration)
	{
		//elapsed = 0;
		m_elapsedTime = 0.f;
		m_shakeOffset.x = 0.f;
		m_shakeOffset.y = 0.f;
		setEnabled(false);
	}
}

void Camera2D::ShakeEffect::setProps(float duration, float speed, float magnitude, float range)
{
	m_duration = duration;
	m_speed = speed;
	m_magnitude = magnitude;
	m_startRange = range;
}

void Camera2D::ShakeEffect::setDuration(float duration)
{
	m_duration = duration;
}

void Camera2D::ShakeEffect::setSpeed(float speed)
{
	m_speed = speed;
}

void Camera2D::ShakeEffect::setMagnitude(float magnitude)
{
	m_magnitude = magnitude;
}

void Camera2D::ShakeEffect::setRange(float range)
{
	m_startRange = range;
}

Camera2D::Vector2 Camera2D::ShakeEffect::getShakeOffset() const
{
	return m_shakeOffset;
}
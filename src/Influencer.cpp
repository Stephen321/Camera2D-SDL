#include "Influencer.h"

void Camera2D::Influencer::setProps(const Vector2 & position, float affectRange)
{
	m_position = position;
	m_affectRange = affectRange;
}

void Camera2D::Influencer::setStrength(float strength)
{
	m_strength = strength;
}

std::string Camera2D::Influencer::getName() const
{
	return m_name;
}

void Camera2D::Influencer::setName(const std::string & name)
{
	m_name = name;
}

Camera2D::Influencer::Type Camera2D::Influencer::getType() const
{
	return m_type;
}

Camera2D::Vector2 Camera2D::Influencer::getPosition() const
{
	return m_position;
}

float Camera2D::Influencer::getRange() const
{
	return m_affectRange;
}

float Camera2D::Influencer::getStrength() const
{
	return m_strength;
}

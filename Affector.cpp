#include "Affector.h"

void Camera2D::Affector::setProps(const Vector2 & position, float affectRange)
{
	m_position = position;
	m_affectRange = affectRange;
}

std::string Camera2D::Affector::getName() const
{
	return m_name;
}

void Camera2D::Affector::setName(const std::string & name)
{
	m_name = name;
}

Camera2D::Affector::Type Camera2D::Affector::getType() const
{
	return m_type;
}

Camera2D::Vector2 Camera2D::Affector::getPosition() const
{
	return m_position;
}

float Camera2D::Affector::getRange() const
{
	return m_affectRange;
}

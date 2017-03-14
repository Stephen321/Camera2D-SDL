#include "Effect.h"

Camera2D::Effect::Effect(Type type)
	: m_type(type)
	, m_enabled(false)
{
}

std::string Camera2D::Effect::getName() const
{
	return m_name;
}

void Camera2D::Effect::setName(const std::string & name)
{
	m_name = name;
}

Camera2D::Effect::Type Camera2D::Effect::getType() const
{
	return m_type;
}

bool Camera2D::Effect::getEnabled() const
{
	return m_enabled;
}

void Camera2D::Effect::setEnabled(bool enabled)
{
	m_enabled = enabled;
}
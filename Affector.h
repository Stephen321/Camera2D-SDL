#pragma once
#include <string>
#include "Vector2.h"
namespace Camera2D
{
	const float DEFAULT_AFFECTOR_RANGE = 300.f;
	class Affector
	{
	public:
		enum class Type
		{
			Attractor,
			Repulsor
		};
		Affector(Type type) : m_type(type) {}
		void setProps(const Vector2& position, float affectRange = DEFAULT_AFFECTOR_RANGE);
		std::string getName() const;
		void setName(const std::string& name);
		Type getType() const;
		Vector2 getPosition() const;
		float getRange() const;
	private:
		Vector2 m_position;
		float m_affectRange;
		std::string m_name;
		Type m_type;
	};
}
#ifndef INFLUENCER_H
#define INFLUENCER_H

#include <string>
#include "Config.h"
#include "Vector2.h"

namespace Camera2D
{
	const float DEFAULT_AFFECTOR_RANGE = 300.f;
	class CAMERA2D_API Influencer
	{
	public:
		enum class Type
		{
			Attractor,
			Repulsor
		};
		Influencer(Type type) : m_type(type) {}
		void setProps(const Vector2& position, float affectRange = DEFAULT_AFFECTOR_RANGE);
		void setStrength(float strength);
		std::string getName() const;
		void setName(const std::string& name);
		Type getType() const;
		Vector2 getPosition() const;
		float getRange() const;
		float getStrength() const;
	protected:
		float m_strength;
	private:
		Vector2 m_position;
		float m_affectRange;
		std::string m_name;
		Type m_type;
	};
}

#endif
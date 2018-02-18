#ifndef INFLUENCER_H
#define INFLUENCER_H

#include <string>
#include "Config.h"
#include "Vector2.h"

namespace Camera2D
{
	const float DEFAULT_AFFECTOR_RANGE = 300.f;
	/*! \brief Base class of influencers.
	*/
	class CAMERA2D_API Influencer
	{
	public:
		/*! \brief The type of the influencer.
		*/
		enum class Type
		{
			Attractor,
			Repulsor
		};
		Influencer(Type type) : m_type(type) {}
		/*! \brief Set the properties of the influencer.
		\param position Position of the influencer.
		\param affectRange Range of the influencer.
		*/
		void setProps(const Vector2& position, float affectRange = DEFAULT_AFFECTOR_RANGE);

		/*! \brief Set the strength of the influencer.
		\param strength How strong the influencer is.
		*/
		void setStrength(float strength);

		/*! \brief Get the name of the influencer.
		\return std::string
		*/
		std::string getName() const;

		/*! \brief Set the name of the influencer.
		\param name Name to set the effect to.
		*/
		void setName(const std::string& name);

		/*! \brief Get the type of the influencer.
		\return Type
		*/
		Type getType() const;

		/*! \brief Get the position of the influencer.
		\return Vector2
		*/
		Vector2 getPosition() const;

		/*! \brief Get the range of the influencer.
		\return float
		*/
		float getRange() const;

		/*! \brief Get the strength of the influencer.
		\return float
		*/
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
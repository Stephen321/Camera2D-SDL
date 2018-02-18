#ifndef EFFECT_H
#define EFFECT_H

#include <string>
#include "Config.h"

namespace Camera2D
{
	/*! \brief Base class of effects.
	*/
	class CAMERA2D_API Effect
	{
	public:
		/*! \brief The type of effect.
		*/
		enum class Type {
			Parallax,
			Shake
		};
		Effect(Type type);
		/*! \brief Get the name of the effect.
		\return std::string
		*/
		std::string getName() const;

		/*! \brief Set the name of effect.
		\param name Name to set the effect to.
		*/
		void setName(const std::string& name);

		/*! \brief Get the type of the effect.
		\return Type
		*/
		Type getType() const;

		/*! \brief Get if the effect is enabled.
		\return bool
		*/
		bool getEnabled() const;

		/*! \brief Set if the effect is enabled.
		\param enabled Set if the effect is enabled.
		*/
		virtual void setEnabled(bool enabled);
	protected:
		std::string m_name;
		Type m_type;
		bool m_enabled;
	};
}

#endif

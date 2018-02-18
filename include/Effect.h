#ifndef EFFECT_H
#define EFFECT_H

#include <string>
#include "Config.h"

namespace Camera2D
{
	class CAMERA2D_API Effect
	{
	public:
		enum class Type {
			Parallax,
			Shake
		};
		Effect(Type type);
		std::string getName() const;
		void setName(const std::string& name);
		Type getType() const;
		bool getEnabled() const;
		virtual void setEnabled(bool enabled);
	protected:
		std::string m_name;
		Type m_type;
		bool m_enabled;
	};
}

#endif

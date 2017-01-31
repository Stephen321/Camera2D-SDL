#pragma once
#include "Influencer.h"
namespace Camera2D
{
	class Attractor : public Influencer
	{
	public:
		Attractor() : Influencer(Type::Attractor) { m_strength = 1.5f; } 
	};
}
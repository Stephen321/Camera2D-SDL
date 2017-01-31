#pragma once
#include "Influencer.h"
namespace Camera2D
{
	class Repulsor : public Influencer
	{
	public:
		Repulsor() : Influencer(Type::Repulsor) { m_strength = 10.f; }
	};
}
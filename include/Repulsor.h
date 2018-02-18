#ifndef REPULSOR_H
#define REPULSOR_H

#include "Influencer.h"
namespace Camera2D
{
	class CAMERA2D_API Repulsor : public Influencer
	{
	public:
		Repulsor() : Influencer(Type::Repulsor) { m_strength = 10.f; }
	};
}

#endif
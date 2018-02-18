#ifndef ATTRACTOR_H
#define ATTRACTOR_H

#include "Influencer.h"
namespace Camera2D
{
	/*! \brief Causes the camera centre to be pulled towards the attractors centre.
	*/
	class CAMERA2D_API Attractor : public Influencer
	{
	public:
		Attractor() : Influencer(Type::Attractor) { m_strength = 1.5f; } 
	};
}

#endif
#pragma once
#include "Affector.h"
namespace Camera2D
{
	class Attractor : public Affector
	{
	public:
		Attractor() : Affector(Type::Attractor) {}
	};
}
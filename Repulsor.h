#pragma once
#include "Affector.h"
namespace Camera2D
{
	class Repulsor : public Affector
	{
	public:
		Repulsor() : Affector(Type::Repulsor) {}
	};
}
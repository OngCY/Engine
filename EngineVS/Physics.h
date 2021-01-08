#pragma once
#include "Physics/particle.h"
#include <matrix4.h>

class ParticleObject
{
public:
	ParticleObject();
	~ParticleObject();

	void SetMass(float mass);
	void SetDamping(float damping);
	void SetAcceleration(float x, float y, float z);
	void SetAcceleration(irr::core::vector3df acceleration);
	void SetVelocity(float x, float y, float z);
	void SetVelocity(irr::core::vector3df velocity);
	void SetPosition(float x, float y, float z);
	void SetPosition(irr::core::vector3df position);

	irr::core::vector3df GetPosition();
	void Update(unsigned long duration);

private:
	cyclone::Particle* m_particle;
};
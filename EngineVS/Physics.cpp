#include "Physics.h"

ParticleObject::ParticleObject()
{
	m_particle = new cyclone::Particle();
}

ParticleObject::~ParticleObject()
{
	delete m_particle;
	m_particle = NULL;
}

void ParticleObject::SetMass(float mass)
{
	m_particle->setMass(mass);
}

void ParticleObject::SetDamping(float damping)
{
	m_particle->setDamping(damping);
}

void ParticleObject::SetAcceleration(float x, float y, float z)
{
	m_particle->setAcceleration(x, y, z);
}

void ParticleObject::SetAcceleration(irr::core::vector3df acceleration)
{
	cyclone::Vector3 vec(acceleration.X, acceleration.Y, acceleration.Z);
	m_particle->setAcceleration(vec);
}

void ParticleObject::SetVelocity(float x, float y, float z)
{
	m_particle->setVelocity(x, y, z);
}

void ParticleObject::SetVelocity(irr::core::vector3df velocity)
{
	cyclone::Vector3 vec(velocity.X, velocity.Y, velocity.Z);
	m_particle->setVelocity(vec);
}

void ParticleObject::SetPosition(float x, float y, float z)
{
	m_particle->setPosition(x, y, z);
}

void ParticleObject::SetPosition(irr::core::vector3df position)
{
	cyclone::Vector3 vec(position.X, position.Y, position.Z);
	m_particle->setPosition(vec);
}

irr::core::vector3df ParticleObject::GetPosition()
{
	irr::core::vector3df vec;
	vec.X = m_particle->getPosition().x;
	vec.Y = m_particle->getPosition().y;
	vec.Z = m_particle->getPosition().z;

	return vec;
}

void ParticleObject::Update(unsigned long duration)
{
	m_particle->integrate((cyclone::real)duration);
}
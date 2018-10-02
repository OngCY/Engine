#include "ActorComponent.h"

BaseActorComponent::BaseActorComponent():m_componentId(0)
{
}

MyTypes::ComponentId BaseActorComponent::VGetComponentId(void) const
{
	return m_componentId;
}

HealthComponent::HealthComponent() :BaseActorComponent(),m_boost(0)
{
}

HealthComponent::~HealthComponent(void)
{
}

bool HealthComponent::VInit(json jHealthComponent, unsigned int id)
{
	m_type = jHealthComponent["type"].get<std::string>();
	m_boost = jHealthComponent["boost"].get<int>();
	m_componentId = id;

	return true;
}

void HealthComponent::VPostInit(void)
{
}

void HealthComponent::VUpdate(int deltaMs)
{
}
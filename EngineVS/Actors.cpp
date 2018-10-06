#include "Actors.h"

///Components
BaseActorComponent::BaseActorComponent():m_componentId(0)
{
}

HealthComponent::HealthComponent() :BaseActorComponent(),m_boost(0)
{
}

bool HealthComponent::VInit(nlohmann::json jHealthComponent, unsigned int id)
{
	m_type = jHealthComponent["type"].get<std::string>();
	m_boost = jHealthComponent["boost"].get<int>();
	m_componentId = id;

	return true;
}

///Actor
BaseActor::BaseActor(MyTypes::ActorId id) :m_actorId(id)
{

}

MyTypes::ActorId BaseActor::GetActorID() const
{
	return m_actorId;
}

void BaseActor::AddComponent(StrongActorComponentPtr pComponent)
{
	m_componentMap.insert({ pComponent->VGetComponentId(),pComponent });
}
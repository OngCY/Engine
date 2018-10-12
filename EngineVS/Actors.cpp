#include "Actors.h"

/*******COMPONENT**********/
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

BaseActorComponent* CreateHealthComponent()
{
	return new HealthComponent;
}

/*******ACTOR**********/
BaseActor::BaseActor(MyTypes::ActorId id) :m_actorId(id)
{

}

MyTypes::ActorId BaseActor::GetActorID() const
{
	return m_actorId;
}

void BaseActor::Destroy()
{
	//clear empties the map but does not deallocate the memory of the individual elements
	m_componentMap.clear();
}

bool BaseActor::Init(nlohmann::json jComponentArray)
{
	std::vector<std::string> componentVector = jComponentArray["Components"];
	for (auto component : componentVector)
	{
		
	}
	
	return true;
}

void BaseActor::PostInit()
{
	ActorComponentMap::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VPostInit();
}

void BaseActor::Update(int deltaMs)
{
	ActorComponentMap::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VUpdate(deltaMs);
}

void BaseActor::AddComponent(StrongActorComponentPtr pComponent)
{
	m_componentMap.insert(std::pair<MyTypes::ComponentId, StrongActorComponentPtr>(pComponent->VGetComponentId(),pComponent));
}

/*******ACTOR FACTORY**********/
ActorFactory::ActorFactory(void)
{
	m_actorComponentCreatorMap["HealthComponent"] = CreateHealthComponent;
}

MyTypes::ActorId ActorFactory::GetNextActorId(void)
{
	++m_lastActorId;

	return m_lastActorId;
}
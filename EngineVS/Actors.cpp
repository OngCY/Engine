#include "Actors.h"
#include <fstream>

/*******COMPONENT**********/
BaseActorComponent::BaseActorComponent():m_componentId(0)
{
}

HealthComponent::HealthComponent() :BaseActorComponent(),m_boost(0)
{
}

bool HealthComponent::VInit(nlohmann::json jHealthComponent)
{
	m_type = jHealthComponent["type"].get<std::string>();
	m_boost = jHealthComponent["boost"].get<int>();

	return true;
}

BaseActorComponent* CreateHealthComponent()
{
	return new HealthComponent;
}

/*******ACTOR**********/
BaseActor::BaseActor(MyTypes::ActorId id) :m_actorId(id), m_lastComponentId(0)
{

}

BaseActor::~BaseActor(void)
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
	++m_lastComponentId;
	pComponent->VSetComponentId(m_lastComponentId);
	m_componentMap.insert(std::pair<MyTypes::ComponentId, StrongActorComponentPtr>(m_lastComponentId,pComponent));
}

/*******ACTOR FACTORY**********/
ActorFactory::ActorFactory(void):m_lastActorId(0)
{
	m_actorComponentCreatorMap["HealthComponent"] = CreateHealthComponent;
}

MyTypes::ActorId ActorFactory::GetNextActorId(void)
{
	++m_lastActorId;

	return m_lastActorId;
}

StrongActorPtr ActorFactory::CreateActor(const char* filePath)
{
	std::ifstream ifs(filePath);
	nlohmann::json jstream = nlohmann::json::parse(ifs);

	if (!jstream)
	{
		std::cout << "Unable to parse actor component file: " << filePath << std::endl;
		return StrongActorPtr();
	}

	StrongActorPtr pActor(new BaseActor(GetNextActorId()));
	if (!pActor->Init())
	{
		std::cout << "Unable to initialise actor: " << filePath << std::endl;
		return StrongActorPtr();
	}

	std::vector<std::string> componentVector = jstream["Components"];
	for (std::string compName : componentVector)
	{
		StrongActorComponentPtr pComponent(CreateComponent(compName, jstream[compName]));

		if (pComponent)
		{
			pActor->AddComponent(pComponent);
			pComponent->SetOwner(pActor);
		}
		else
			return StrongActorPtr();
	}

	pActor->PostInit();

	return pActor;
}

StrongActorComponentPtr ActorFactory::CreateComponent(std::string compName, nlohmann::json jComponent)
{
	StrongActorComponentPtr pComponent;

	ActorComponentCreatorMap::iterator it = m_actorComponentCreatorMap.find(compName);
	if (it != m_actorComponentCreatorMap.end())
	{
		ActorComponentCreator compCreator = it->second;
		pComponent.reset(compCreator());
	}
	else
	{
		std::cout << "Cannot find ActorComponent named: " << compName << std::endl;
		return StrongActorComponentPtr();
	}
	
	if (pComponent)
	{
		if (!pComponent->VInit(jComponent))
		{
			std::cout << "Component could not initialise: " << compName << std::endl;
			return StrongActorComponentPtr();
		}
	}
		
	return pComponent;
}
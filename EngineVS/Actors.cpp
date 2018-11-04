#include "Actors.h"
#include <fstream>

/*******HEALTH COMPONENT**********/
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

/*******BASE ACTOR**********/
void BaseActor::Destroy()
{
	//clear empties the map but does not deallocate the memory of the individual elements
	m_componentMap.clear();
}

void BaseActor::PostInit()
{
	ComponentMap::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VPostInit();
}

void BaseActor::Update(int deltaMs)
{
	ComponentMap::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VUpdate(deltaMs);
}

void BaseActor::AddComponent(StrongActorComponentPtr pComponent)
{
	++m_lastComponentId; //The component id is determined by its Actor owner
	
	pComponent->VSetComponentId(m_lastComponentId);
	m_componentMap.insert(std::pair<MyTypes::ComponentId, StrongActorComponentPtr>(m_lastComponentId,pComponent));
}

/*******ACTOR FACTORY**********/
ActorFactory::ActorFactory(void):m_lastActorId(0)
{
	m_actorComponentCreatorMap["HealthComponent"] = CreateHealthComponent; //map of component name to function pointer returning a new component object
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

	//Create the actor and assign an actor ID
	StrongActorPtr pActor(new BaseActor(GetNextActorId()));
	if (!pActor->Init())
	{
		std::cout << "Unable to initialise actor: " << filePath << std::endl;
		return StrongActorPtr();
	}

	//Create the actor's components
	std::vector<std::string> componentVector = jstream["Components"]; //get list of components from the json stream
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

	//Create a new component object from a function pointer. Identify the function pointer by the component name
	ActorComponentCreatorMap::iterator it = m_actorComponentCreatorMap.find(compName);
	if (it != m_actorComponentCreatorMap.end())
	{
		ActorComponentCreator compCreator = it->second;
		pComponent.reset(compCreator()); //reset replaces the pComponent object with the object returned by the component creator
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
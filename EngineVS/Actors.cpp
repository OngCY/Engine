#include "Actors.h"
#include <fstream>

//define static constants in the cpp file
const MyTypes::ComponentId HealthPickUp::COMPONENT_ID = COMPONENTS::PICKUP_HEALTH;
const MyTypes::ComponentId HealthLifeComponent::COMPONENT_ID = COMPONENTS::HEALTH_LIFE;

/*******HEALTH COMPONENT**********/
bool HealthPickUp::VInit(nlohmann::json jHealthComponent)
{
	m_type = jHealthComponent["type"].get<std::string>();
	m_boost = jHealthComponent["boost"].get<int>();
	std::cout << "Health Pickup:VInit" << std::endl;
	std::cout << "type:" << m_type << " boost:" << m_boost << std::endl;
	return true;
}

void HealthPickUp::VApply(WeakActorPtr pActor)
{
	std::cout << "Health Pickup:VApply" << std::endl;
	std::cout << "Apply green herb pickup to receipient pActor" << std::endl;
	std::cout << "Remove green herb from scene" << std::endl;
}

int HealthPickUp::VGetHealthBoost()
{
	std::cout << "Health Pickup:VGetHealthBoost" << std::endl;
	return m_boost;
}

BaseActorComponent* CreateHealthPickUp()
{
	return new HealthPickUp;
}

/*******HEALTH LIFE COMPONENT**********/
bool HealthLifeComponent::VInit(nlohmann::json jHealthLifeComponent)
{
	m_state = jHealthLifeComponent["state"].get<std::string>();
	m_health = jHealthLifeComponent["health"].get<int>();
	std::cout << "HealthLifeComponent:VInit" << std::endl;
	std::cout << "state:" << m_state << " health:" << m_health << std::endl;
	return true;
}

void HealthLifeComponent::VUpdateHealth(int health)
{
	std::cout << "HealthLifeComponent:VUpdateHealth" << std::endl;
	m_health += health;
	std::cout << "Health is now: " << m_health << std::endl;
}

BaseActorComponent* CreateHealthLifeComponent()
{
	return new HealthLifeComponent;
}

/*******BASE ACTOR**********/
void BaseActor::Destroy()
{
	m_componentMap.clear(); //clear empties the map but does not deallocate the memory of the individual elements
}

void BaseActor::PostInit()
{
	std::cout << "BaseActor:PostInit" << std::endl;
	ComponentMap::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VPostInit();
}

void BaseActor::Update(int deltaMs)
{
	std::cout << "BaseActor:Update" << std::endl;
	ComponentMap::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VUpdate(deltaMs);
}

void BaseActor::AddComponent(StrongActorComponentPtr pComponent)
{
	std::cout << "BaseActor:AddComponent" << std::endl;
	MyTypes::ComponentId compId = pComponent->VGetComponentId();
	std::cout << "ComponetID:" << compId << std::endl;
	m_componentMap.insert(std::pair<MyTypes::ComponentId, StrongActorComponentPtr>(compId,pComponent));
}

/*******ACTOR FACTORY**********/
ActorFactory::ActorFactory(void):m_lastActorId(0)
{
	std::cout << "ActorFactory: constructor" << std::endl;
	m_actorComponentCreatorMap["HealthPickUp"] = CreateHealthPickUp; //map of component name to function pointer returning a new component object
	m_actorComponentCreatorMap["HealthLifeComponent"] = CreateHealthLifeComponent;
}

MyTypes::ActorId ActorFactory::GetNextActorId(void)
{
	std::cout << "ActorFactory:GetNextActorId" << std::endl;
	++m_lastActorId;
	
	return m_lastActorId;
}

StrongActorPtr ActorFactory::CreateActor(const char* filePath)
{
	std::cout << "ActorFactory:CreateActor" << std::endl;
	std::ifstream ifs(filePath);
	nlohmann::json jstream;

	try
	{
		jstream = nlohmann::json::parse(ifs);
	}
	catch (nlohmann::json::parse_error& e)
	{
		std::cout << "Unable to parse actor component file: " << filePath << std::endl;
		std::cout << "parse error message: " << e.what() << "error id: " << e.id << std::endl;
		return StrongActorPtr();
	}

	//Create the actor and assign an actor ID
	std::cout << "Creating and initialising actor" << std::endl;
	StrongActorPtr pActor(new BaseActor(GetNextActorId()));
	if (!pActor->Init())
	{
		std::cout << "Unable to initialise actor: " << filePath << std::endl;
		return StrongActorPtr();
	}

	//Create the actor's components
	std::cout << "Creating components for the actor" << std::endl;
	std::vector<std::string> componentVector = jstream["Components"]; //get list of components from the json stream
	for (std::string compName : componentVector)
	{
		std::cout << "Creating Component:" << compName << std::endl;
		StrongActorComponentPtr pComponent(CreateComponent(compName, jstream[compName]));

		if (pComponent)
		{
			std::cout << "Adding component to actor" << std::endl;
			pActor->AddComponent(pComponent);
			pComponent->SetOwner(pActor);
		}
		else
		{
			std::cout << "Component not created. Returning empty actor" << std::endl;
			return StrongActorPtr();
		}
	}

	std::cout << "Post init actor" << std::endl;
	pActor->PostInit();

	return pActor;
}

StrongActorComponentPtr ActorFactory::CreateComponent(std::string compName, nlohmann::json jComponent)
{
	std::cout << "ActorFactory:CreateComponent" << std::endl;
	StrongActorComponentPtr pComponent;

	//Create a new component object from a function pointer. Identify the function pointer by the component name
	ActorComponentCreatorMap::iterator it = m_actorComponentCreatorMap.find(compName);
	if (it != m_actorComponentCreatorMap.end())
	{
		std::cout << "Creating component from component creator:" << compName << std::endl;
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
		std::cout << "Initialising component" << std::endl;
		if (!pComponent->VInit(jComponent))
		{
			std::cout << "Component could not initialise: " << compName << std::endl;
			return StrongActorComponentPtr();
		}
	}
		
	return pComponent;
}
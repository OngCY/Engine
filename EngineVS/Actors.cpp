#include "Actors.h"
#include <fstream>

//define static constants in the cpp file
const MyTypes::ComponentId HealthPickUp::COMPONENT_ID = COMPONENTS::PICKUP_HEALTH;
const MyTypes::ComponentId HealthLifeComponent::COMPONENT_ID = COMPONENTS::HEALTH_LIFE;
const MyTypes::ComponentId TranslateComponent::COMPONENT_ID = COMPONENTS::TRANSLATE;

/**********COMPONENTS**********/
void BaseActorComponent::SetOwner(StrongActorPtr_t pOwner)
{
	m_pOwner = pOwner;
}

/////
bool HealthPickUp::VInit(nlohmann::json jHealthComponent)
{
	m_type = jHealthComponent["type"].get<std::string>();
	m_boost = jHealthComponent["boost"].get<int>();
	
	return true;
}

MyTypes::ComponentId HealthPickUp::VGetComponentId(void) const
{ 
	return HealthPickUp::COMPONENT_ID; 
}

int HealthPickUp::VGetHealthBoost()
{
	return m_boost;
}

BaseActorComponent* CreateHealthPickUp()
{
	return new HealthPickUp;
}

/////
bool HealthLifeComponent::VInit(nlohmann::json jHealthLifeComponent)
{
	m_state = jHealthLifeComponent["state"].get<std::string>();
	m_health = jHealthLifeComponent["health"].get<int>();

	return true;
}

MyTypes::ComponentId HealthLifeComponent::VGetComponentId(void) const
{
	return HealthLifeComponent::COMPONENT_ID;
}

void HealthLifeComponent::VUpdateHealth(int health)
{
	m_health += health;
}

BaseActorComponent* CreateHealthLifeComponent()
{
	return new HealthLifeComponent;
}

/////
bool TranslateComponent::VInit(nlohmann::json jTranslateComponent)
{
	//build translation matrix
	m_translateType = jTranslateComponent["type"].get<int>();
	return true;
}

MyTypes::ComponentId TranslateComponent::VGetComponentId(void) const
{
	return TranslateComponent::COMPONENT_ID;
}

void TranslateComponent::VApplyTranslation(unsigned int type)
{
	//apply transform to m_pOwner
	std::cout << "Translation applied to actor" << std::endl;
}

BaseActorComponent* CreateTranslateComponent()
{
	return new TranslateComponent;
}

/**********ACTOR**********/
void BaseActor::Destroy()
{
	m_componentMap.clear(); //clear empties the map but does not deallocate the memory of the individual elements
}

void BaseActor::PostInit()
{
	ComponentMap_t::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VPostInit();
}

void BaseActor::Update(int deltaMs) //update the actor's components every game loop
{
	ComponentMap_t::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VUpdate(deltaMs);
}

void BaseActor::AddComponent(StrongActorComponentPtr_t pComponent)
{
	MyTypes::ComponentId compId = pComponent->VGetComponentId();
	m_componentMap.insert(std::pair<MyTypes::ComponentId, StrongActorComponentPtr_t>(compId,pComponent));
}

/**********ACTOR FACTORY**********/
ActorFactory::ActorFactory(void):m_lastActorId(0)
{
	m_actorComponentCreatorMap["HealthPickUp"] = CreateHealthPickUp; //map component name to function pointer returning a new component object
	m_actorComponentCreatorMap["HealthLifeComponent"] = CreateHealthLifeComponent;
	m_actorComponentCreatorMap["TranslateComponent"] = CreateTranslateComponent;
}

MyTypes::ActorId ActorFactory::GetNextActorId(void)
{
	++m_lastActorId; //the actor factory supplies the actor ID
	
	return m_lastActorId;
}

StrongActorPtr_t ActorFactory::CreateActor(const char* filePath, MyTypes::ActorId actorId)
{
	std::ifstream ifs(filePath);
	nlohmann::json jstream;

	try
	{
		jstream = nlohmann::json::parse(ifs);
	}
	catch (nlohmann::json::parse_error& e)
	{
		ServiceLocator::GetLogService()->VGetLogger()->error("Error parsing actor component file: {}", filePath);
		ServiceLocator::GetLogService()->VGetLogger()->error("Parse error message: {} error ID: {}", e.what(), e.id);
		
		return StrongActorPtr_t(); //return an empty actor pointer in the case of exceptions
	}

	//Create an actor object and assign an actor ID
	StrongActorPtr_t pActor(new BaseActor(actorId));
	if (!pActor->Init())
	{
		ServiceLocator::GetLogService()->VGetLogger()->error("Unable to initialise actor");
		return StrongActorPtr_t();
	}

	//Create the actor's components from the json stream
	std::vector<std::string> componentVector = jstream["Components"]; //get the component names from the json stream
	for (std::string compName : componentVector)
	{
		StrongActorComponentPtr_t pComponent(CreateComponent(compName, jstream[compName])); //instantiate each component

		if (pComponent)
		{
			pActor->AddComponent(pComponent);
			pComponent->SetOwner(pActor);
		}
		else
		{
			return StrongActorPtr_t();
		}
	}

	pActor->PostInit();

	return pActor;
}

StrongActorComponentPtr_t ActorFactory::CreateComponent(std::string compName, nlohmann::json jComponent)
{
	StrongActorComponentPtr_t pComponent;

	//Create a new component object from a function pointer. Identify the function pointer by the component name
	ActorComponentCreatorMap_t::iterator it = m_actorComponentCreatorMap.find(compName);
	if (it != m_actorComponentCreatorMap.end())
	{
		ActorComponentCreator_t compCreator = it->second; //get the component creator function pointer
		pComponent.reset(compCreator()); //reset replaces the pComponent object with the object returned by the component creator
	}
	else
	{
		ServiceLocator::GetLogService()->VGetLogger()->error("Cannot find actor component named: {}", compName);
		return StrongActorComponentPtr_t();
	}
	
	if (pComponent)
	{
		if (!pComponent->VInit(jComponent))
		{
			ServiceLocator::GetLogService()->VGetLogger()->error("Component could not be initialised: {}", compName);
			return StrongActorComponentPtr_t();
		}
	}
		
	return pComponent;
}
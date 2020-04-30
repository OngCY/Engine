#include "Actors.h"
#include <fstream>

//define static constants in the cpp file
const MyTypes::ComponentId HealthPickUp::COMPONENT_ID = COMPONENTS::PICKUP_HEALTH;
const MyTypes::ComponentId TransformComponent::COMPONENT_ID = COMPONENTS::TRANSFORM;
const MyTypes::ComponentId HealthLifeComponent::COMPONENT_ID = COMPONENTS::HEALTH_LIFE;

//////////////////////COMPONENTS/////////////////////
/**********HEALTH PICKUP COMPONENT**********/
bool HealthPickUp::VInit(nlohmann::json jHealthComponent)
{
	m_type = jHealthComponent["type"].get<std::string>();
	m_boost = jHealthComponent["boost"].get<int>();
	
	return true;
}

void HealthPickUp::VApplyPickup(WeakActorPtr pActor)
{
	return;
}

int HealthPickUp::VGetHealthBoost()
{
	return m_boost;
}

BaseActorComponent* CreateHealthPickUp()
{
	return new HealthPickUp;
}

/**********HEALTH LIFE COMPONENT**********/
bool HealthLifeComponent::VInit(nlohmann::json jHealthLifeComponent)
{
	m_state = jHealthLifeComponent["state"].get<std::string>();
	m_health = jHealthLifeComponent["health"].get<int>();

	return true;
}

void HealthLifeComponent::VUpdateHealth(int health)
{
	m_health += health;
}

BaseActorComponent* CreateHealthLifeComponent()
{
	return new HealthLifeComponent;
}

/**********TRANSFORM COMPONENT**********/
bool TransformComponent::VInit(nlohmann::json jTransformComponent)
{
	m_translateX = jTransformComponent["translateX"].get<float>();
	m_translateY = jTransformComponent["translateY"].get<float>();
	m_translateZ = jTransformComponent["translateZ"].get<float>();
	
	m_rotateX = jTransformComponent["rotateX"].get<float>();
	m_rotateY = jTransformComponent["rotateY"].get<float>();
	m_rotateZ = jTransformComponent["rotateZ"].get<float>();
	
	m_scaleX = jTransformComponent["scaleX"].get<float>();
	m_scaleY = jTransformComponent["scaleY"].get<float>();
	m_scaleZ = jTransformComponent["scaleZ"].get<float>();

	return true;
}

void TransformComponent::VApplyTransform()
{
	//apply transform to m_pOwner
	std::cout << "Transform applied to actor" << std::endl;
}

BaseActorComponent* CreateTransformComponent()
{
	return new TransformComponent;
}

//////////////////////BASE ACTOR/////////////////////
void BaseActor::Destroy()
{
	m_componentMap.clear(); //clear empties the map but does not deallocate the memory of the individual elements
}

void BaseActor::PostInit()
{
	ComponentMap::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VPostInit();
}

void BaseActor::Update(int deltaMs) //update the actor's components every game loop
{
	ComponentMap::iterator it;
	for (it = m_componentMap.begin(); it != m_componentMap.end(); ++it)
		it->second->VUpdate(deltaMs);
}

void BaseActor::AddComponent(StrongActorComponentPtr pComponent)
{
	MyTypes::ComponentId compId = pComponent->VGetComponentId();
	m_componentMap.insert(std::pair<MyTypes::ComponentId, StrongActorComponentPtr>(compId,pComponent));
}

//////////////////////ACTOR FACTORY/////////////////////
ActorFactory::ActorFactory(void):m_lastActorId(0)
{
	m_actorComponentCreatorMap["HealthPickUp"] = CreateHealthPickUp; //map component name to function pointer returning a new component object
	m_actorComponentCreatorMap["HealthLifeComponent"] = CreateHealthLifeComponent;
	m_actorComponentCreatorMap["TransformComponent"] = CreateTransformComponent;
}

MyTypes::ActorId ActorFactory::GetNextActorId(void)
{
	++m_lastActorId; //the actor factory supplies the actor ID
	
	return m_lastActorId;
}

StrongActorPtr ActorFactory::CreateActor(const char* filePath, MyTypes::ActorId actorId)
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
		
		return StrongActorPtr(); //return an empty actor pointer in the case of exceptions
	}

	//Create an actor object and assign an actor ID
	StrongActorPtr pActor(new BaseActor(actorId));
	if (!pActor->Init())
	{
		ServiceLocator::GetLogService()->VGetLogger()->error("Unable to initialise actor");
		return StrongActorPtr();
	}

	//Create the actor's components from the json stream
	std::vector<std::string> componentVector = jstream["Components"]; //get the component names from the json stream
	for (std::string compName : componentVector)
	{
		StrongActorComponentPtr pComponent(CreateComponent(compName, jstream[compName])); //instantiate each component

		if (pComponent)
		{
			pActor->AddComponent(pComponent);
			pComponent->SetOwner(pActor);
		}
		else
		{
			return StrongActorPtr();
		}
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
		ActorComponentCreator compCreator = it->second; //get the component creator function pointer
		pComponent.reset(compCreator()); //reset replaces the pComponent object with the object returned by the component creator
	}
	else
	{
		ServiceLocator::GetLogService()->VGetLogger()->error("Cannot find actor component named: {}", compName);
		return StrongActorComponentPtr();
	}
	
	if (pComponent)
	{
		if (!pComponent->VInit(jComponent))
		{
			ServiceLocator::GetLogService()->VGetLogger()->error("Component could not be initialised: {}", compName);
			return StrongActorComponentPtr();
		}
	}
		
	return pComponent;
}
#pragma once
#include "MyTypes.h"
#include "ServiceLocator.h"
#include "ThirdParty\json.hpp"
#include <memory>
#include <iostream>
#include <matrix4.h>

class BaseActor;
class BaseActorComponent;

//typedefs for BaseActor
typedef std::shared_ptr<BaseActor> StrongActorPtr;
typedef std::weak_ptr<BaseActor> WeakActorPtr;
typedef std::shared_ptr<BaseActorComponent> StrongActorComponentPtr;
typedef std::map<MyTypes::ComponentId, StrongActorComponentPtr> ComponentMap;

//typedefs for ActorFactory
typedef BaseActorComponent* (*ActorComponentCreator)(void); //function pointer typedef. Returns a BaseActorComponent* and accepts no parameters
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;

/*******COMPONENTS**********/
class BaseActorComponent
{
	friend class ActorFactory;

public:
	virtual ~BaseActorComponent(void) {}
	virtual bool VInit(nlohmann::json jComponent) = 0;
	virtual void VPostInit(void) = 0;
	virtual void VUpdate(int deltaMS) = 0;
	virtual MyTypes::ComponentId VGetComponentId(void) const = 0;

protected:	
	StrongActorPtr m_pOwner;
	
private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

class IPickUpComponent : public BaseActorComponent
{
public:
	virtual ~IPickUpComponent(void) {}
	virtual bool VInit(nlohmann::json jComponent) = 0;
	virtual void VPostInit(void) = 0;
	virtual void VUpdate(int deltaMS) = 0;
	virtual MyTypes::ComponentId VGetComponentId(void) const = 0;
	
	virtual void VApplyPickup(WeakActorPtr pActor) = 0;
};

class HealthPickUp : public IPickUpComponent
{
public:
	const static MyTypes::ComponentId COMPONENT_ID; //unique id for this component type
	
	HealthPickUp() { m_boost = 0; }
	virtual ~HealthPickUp(void) {}
	virtual bool VInit(nlohmann::json jComponent);
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMS) {}
	virtual MyTypes::ComponentId VGetComponentId(void) const { return COMPONENT_ID; }
	
	virtual void VApplyPickup(WeakActorPtr pActor);
	virtual int VGetHealthBoost();

private:
	short int m_boost;
	std::string m_type;
};

class HealthLifeComponent : public BaseActorComponent
{
public:
	const static MyTypes::ComponentId COMPONENT_ID;
	
	HealthLifeComponent() { m_state = "OK"; m_health = 100; }
	virtual ~HealthLifeComponent(void) {}
	virtual bool VInit(nlohmann::json jComponent);
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMS) {}
	virtual MyTypes::ComponentId VGetComponentId(void) const { return COMPONENT_ID; }
	
	virtual void VUpdateHealth(int health);

private:
	std::string m_state;
	int m_health;
};

class TranslateComponent : public BaseActorComponent
{
public:
	const static MyTypes::ComponentId COMPONENT_ID; //unique id for this component type

	TranslateComponent();
	virtual ~TranslateComponent(void) {}
	virtual bool VInit(nlohmann::json jComponent);
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMS) {}
	virtual MyTypes::ComponentId VGetComponentId(void) const { return COMPONENT_ID; }
	virtual void VApplyTranslation();

private:
	MyTypes::TranslateType m_translateType;
};

/*******COMPONENT CREATORS**********/
BaseActorComponent* CreateHealthPickUp();
BaseActorComponent* CreateHealthLifeComponent();
BaseActorComponent* CreateTransformComponent();

/*******ACTOR**********/
class BaseActor
{
	friend class ActorFactory;
	
public:
	explicit BaseActor(MyTypes::ActorId id) { m_actorId = 0; }
	~BaseActor(void) {}

	bool Init() { return true; }
	void PostInit(void);
	void Destroy(void);
	void Update(int deltaMs);
	MyTypes::ActorId GetActorID(void) const { return m_actorId; }

	template<class ComponentType>
	std::weak_ptr<ComponentType> GetComponent(MyTypes::ComponentId id)
	{
		ComponentMap::iterator it = m_componentMap.find(id);
		if (it != m_componentMap.end())
		{
			StrongActorComponentPtr pBaseComp(it->second);

			//std::shared_ptr<ComponentType> pActualComp(std::tr1::static_pointer_cast<ComponentType>(pBaseComp));
			std::shared_ptr<ComponentType> pActualComp(std::static_pointer_cast<ComponentType>(pBaseComp));
			std::weak_ptr<ComponentType> pWeakActualComp(pActualComp);

			return pWeakActualComp;
		}
		else
			return std::weak_ptr<ComponentType>();
	}

private:
	void AddComponent(StrongActorComponentPtr pComponent);
	
	ComponentMap m_componentMap;
	MyTypes::ActorId m_actorId;
};

/*******ACTOR FACTORY**********/
class ActorFactory
{
public:
	ActorFactory(void);
	StrongActorPtr CreateActor(const char* filePath, MyTypes::ActorId actorId);
	
private:
	StrongActorComponentPtr CreateComponent(std::string compName, nlohmann::json jComponent);
	MyTypes::ActorId GetNextActorId(void);
	
	MyTypes::ActorId m_lastActorId;
	ActorComponentCreatorMap m_actorComponentCreatorMap;
};
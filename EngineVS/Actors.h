#pragma once
#include "MyTypes.h"
#include "ThirdParty\json.hpp"
#include <memory>
#include <iostream>

class BaseActor;
class BaseActorComponent;

typedef std::shared_ptr<BaseActor> StrongActorPtr;
typedef std::shared_ptr<BaseActorComponent> StrongActorComponentPtr;

//typedef for BaseActor
typedef std::map<MyTypes::ComponentId, StrongActorComponentPtr> ComponentMap;

//typedefs for ActorFactory
typedef BaseActorComponent* (*ActorComponentCreator)(void); //function pointer typedef. Returns a BaseActorComponent* and accepts no parameters
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;

class BaseActorComponent
{
	friend class ActorFactory;

public:
	BaseActorComponent() { m_componentId = 0; }
	virtual ~BaseActorComponent(void) {}

	virtual bool VInit(nlohmann::json jComponent) { return true; };
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMS) {}
	virtual void VSetComponentId(MyTypes::ComponentId id) { m_componentId = id; }
	virtual MyTypes::ComponentId VGetComponentId(void) const { return m_componentId; }

protected:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
	
	StrongActorPtr m_pOwner;
	unsigned int m_componentId;	
};

/*******INTERFACE FOR PICKUP COMPONENTS**********/
class IPickUpComponent : public BaseActorComponent
{
public:
	IPickUpComponent() {}
	virtual ~IPickUpComponent(void) {}
	virtual bool VInit(nlohmann::json jComponent) { return true; }
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMS) {}
};

/*******PICKUP COMPONENTS**********/
class HealthComponent : public IPickUpComponent
{
public:
	HealthComponent() { m_boost = 0; }
	virtual ~HealthComponent(void) {}
	virtual bool VInit(nlohmann::json jComponent);
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMS) {}

private:
	short int m_boost;
	std::string m_type;
};

/*******COMPONENT CREATOR FUNCTIONS**********/
BaseActorComponent* CreateHealthComponent();

class BaseActor
{
	friend class ActorFactory;
	
public:
	explicit BaseActor(MyTypes::ActorId id) { m_actorId = m_lastComponentId = 0; }
	~BaseActor(void) {}

	bool Init() { return true; }
	void PostInit(void);
	void Destroy(void);
	void Update(int deltaMs);
	MyTypes::ActorId GetActorID(void) const { return m_actorId; }

	template<class ComponentType>
	std::weak_ptr<ComponentType> GetComponent(MyTypes::ComponentId id)
	{
		ActorComponentsMap::iterator it = m_componentMap.find(id);
		if (it != m_componentMap.end())
		{
			StrongActorComponentPtr pBaseComp(it->second);

			std::shared_ptr<ComponentType> pActualComp(std::tr1::static_pointer_cast<ComponentType>(pBaseComp));
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
	MyTypes::ComponentId m_lastComponentId;
};

class ActorFactory
{
public:
	ActorFactory(void);
	StrongActorPtr CreateActor(const char* filePath);
	
private:
	StrongActorComponentPtr CreateComponent(std::string compName, nlohmann::json jComponent);
	MyTypes::ActorId GetNextActorId(void);
	
	MyTypes::ActorId m_lastActorId;
	ActorComponentCreatorMap m_actorComponentCreatorMap;
};
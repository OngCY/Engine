#pragma once
#include "MyTypes.h"
#include "ThirdParty\json.hpp"
#include <memory>
#include <iostream>

class BaseActor;
class BaseActorComponent;

typedef std::shared_ptr<BaseActor> StrongActorPtr;
typedef std::shared_ptr<BaseActorComponent> StrongActorComponentPtr;
typedef std::map<MyTypes::ComponentId, StrongActorComponentPtr> ActorComponentMap;
typedef BaseActorComponent* (*ActorComponentCreator)(void);
typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;

class BaseActorComponent
{
	friend class ActorFactory;

public:
	BaseActorComponent();
	virtual ~BaseActorComponent(void) {}
	virtual bool VInit(nlohmann::json jComponent) { return true; };
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMS) {}
	virtual void VSetComponentId(MyTypes::ComponentId id) { m_componentId = id; }
	virtual MyTypes::ComponentId VGetComponentId(void) const { return m_componentId; }

protected:
	unsigned int m_componentId;
	StrongActorPtr m_pOwner;
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

class HealthComponent : public BaseActorComponent
{
public:
	HealthComponent();
	virtual ~HealthComponent(void) {}
	virtual bool VInit(nlohmann::json jComponent);
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMS) {}

private:
	short int m_boost;
	std::string m_type;
};

BaseActorComponent* CreateHealthComponent();

class BaseActor
{
	friend class ActorFactory;
	
public:
	explicit BaseActor(MyTypes::ActorId id);
	~BaseActor(void);

	bool Init() { return true; }
	void PostInit(void);
	void Destroy(void);
	void Update(int deltaMs);
	MyTypes::ActorId GetActorID(void) const;

	template<class ComponentType>
	std::weak_ptr<ComponentType> GetComponent(MyTypes::ComponentId id)
	{
		return std::weak_ptr<ComponentType>();
	}

private:
	void AddComponent(StrongActorComponentPtr pComponent);
	
	MyTypes::ActorId m_actorId;
	MyTypes::ComponentId m_lastComponentId;
	ActorComponentMap m_componentMap;
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
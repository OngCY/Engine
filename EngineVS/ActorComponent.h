#pragma once
#include "MyTypes.h"
#include "ThirdParty\json.hpp"

using json = nlohmann::json;

class IActorComponent
{
public:
	virtual bool VInit(json jComponent, unsigned int id) = 0;
	virtual void VPostInit(void) = 0;
	virtual void VUpdate(int deltaMs) = 0;
	virtual MyTypes::ComponentId VGetComponentId(void) const = 0;
};

class BaseActorComponent : public IActorComponent
{
	//friend class ActorFactory;

public:
	BaseActorComponent();
	virtual ~BaseActorComponent(void) {}
	virtual bool VInit(json jComponent, unsigned int id) = 0;
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMS) {}
	virtual MyTypes::ComponentId VGetComponentId(void) const;

protected:
	unsigned int m_componentId;
	//StrongActorPtr m_pOwner;
	//void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

class HealthComponent : public BaseActorComponent
{
public:
	HealthComponent();
	virtual ~HealthComponent(void);
	virtual bool VInit(json jComponent, unsigned int id);
	virtual void VPostInit(void);
	virtual void VUpdate(int deltaMS);

private:
	short int m_boost;
	std::string m_type;
};
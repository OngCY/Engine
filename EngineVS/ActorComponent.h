#pragma once
#include "MyTypes.h"
#include "ThirdParty\json.hpp"

using json = nlohmann::json;

class IActorComponent
{
public:
	virtual bool VInit(json jComponent) = 0;
	virtual void VPostInit(void) = 0;
	virtual void VUpdate(int deltaMs) = 0;
	virtual MyTypes::ComponentId VGetComponentId(void) const = 0;
};

class BaseActorComponent : public IActorComponent
{
	//friend class ActorFactory;

public:
	virtual ~BaseActorComponent(void) {}
	virtual bool VInit(json jComponent) {}
	virtual void VPostInit(void) {}
	virtual MyTypes::ComponentId VGetComponentId(void) const = 0;

protected:
	//StrongActorPtr m_pOwner;

private:
	//void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};
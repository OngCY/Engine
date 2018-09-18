#pragma once
#include "MyTypes.h"

class ActorComponent
{
	//friend class ActorFactory;

protected:
	//StrongActorPtr m_pOwner;

public:
	virtual ~ActorComponent(void) {}
	//virtual bool VInit(TiXmlElement* pData) = 0;
	virtual void VPostInit(void) {}
	virtual void VUpdate(int deltaMs) {}
	virtual MyTypes::ComponentId VGetComponentId(void) const = 0;

private:
	//void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};
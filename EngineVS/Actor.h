#pragma once
#include "MyTypes.h"
#include "ThirdParty\json.hpp"
#include "ActorComponent.h"
#include <map>
#include <memory>

typedef std::shared_ptr<BaseActorComponent> StrongActorComponentPtr;
typedef std::map<MyTypes::ComponentId, StrongActorComponentPtr> ActorComponentMap;

class BaseActor
{
	//friend class ActorFactory;
	

public:
	explicit BaseActor(MyTypes::ActorId id);
	~BaseActor(void);

	bool Init(nlohmann::json jComponent);
	void PostInit(void);
	void Destroy(void);
	void Update(int deltaMs);
	MyTypes::ActorId GetActorID(void) const;

private:
	MyTypes::ActorId m_id;
	ActorComponentMap m_componentMap;
	void AddComponent(StrongActorComponentPtr pComponent);
};

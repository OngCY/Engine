#pragma once
#include "MyTypes.h"
#include "ThirdParty\json.hpp"
#include <map>

using json = nlohmann::json;

class Actor
{
	//friend class ActorFactory;

	//typedef std::map<MyTypes::ComponentId, StrongActorComponentPtr> ActorComponents;

public:
	explicit Actor(MyTypes::ActorId id);
	~Actor(void);

	bool Init(json jcomponent);
	void PostInit(void);
	void Destroy(void);
	void Update(int deltaMs);
	MyTypes::ActorId GetActorID(void) const;

private:
	MyTypes::ActorId m_id;
	//ActorComponents m_componentMap;
	//void AddComponent(StrongActorComponentPtr pComponent);
};

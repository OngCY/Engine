#pragma once
#include "ThirdParty\FastDelegate.h"
#include "ThirdParty\FastDelegateBind.h"
#include "ServiceLocator.h"
#include "Actors.h"
#include <iostream>

class ActorFactory;

typedef std::map<MyTypes::ActorId, StrongActorPtr> ActorMap_t;

class GameLogic
{
public:
	GameLogic() {}
	~GameLogic() {}
	
	bool Init(void);
	void Cleanup(void);

	StrongActorPtr CreateActor(std::string filePath, MyTypes::ActorId);
	WeakActorPtr GetActor(MyTypes::ActorId id);

	//event delegates
	void OnEvtTransformActor(IEventPtr pEvent); //event callback function

private:
	ActorFactory* CreateActorFactory(void);
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);

	ActorMap_t m_actors;
	ActorFactory* m_pActorFactory;
};
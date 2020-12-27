#pragma once
#include "ThirdParty\FastDelegate.h"
#include "ThirdParty\FastDelegateBind.h"
#include "ServiceLocator.h"
#include "Actors.h"
#include <iostream>

class ActorFactory;

typedef std::map<MyTypes::ActorId, StrongActorPtr_t> ActorMap_t;

class GameLogic
{
public:
	GameLogic() {}
	~GameLogic() {}
	
	bool Init(void);
	void Cleanup(void);

	WeakActorPtr_t GetActor(MyTypes::ActorId id);

	//event delegates
	void OnEvtTranslatePlayer(IEventPtr_t pEvent); //event callback function

private:
	ActorFactory* CreateActorFactory(void);
	StrongActorPtr_t CreateActor(std::string filePath, MyTypes::ActorId);
	void RegisterPlayerDelegates(void);
	void RemovePlayerDelegates(void);

	ActorMap_t m_actors;
	ActorFactory* m_pActorFactory;
};
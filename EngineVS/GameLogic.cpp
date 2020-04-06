#include "GameLogic.h"

GameLogic::~GameLogic()
{
	EventListenerDelegate delegateFunc = fastdelegate::MakeDelegate(this, &GameLogic::DestroyActorDelegate);
	ServiceLocator::GetEventService()->VRemoveListener(delegateFunc, Event_DestroyActor::sk_EventId);
}

//create a delegate for the call back and register the delegate with the event manager
void GameLogic::Init(void)
{
	EventListenerDelegate delegateListener = fastdelegate::MakeDelegate(this, &GameLogic::DestroyActorDelegate);
	ServiceLocator::GetEventService()->VRegisterListener(delegateListener, Event_DestroyActor::sk_EventId);
}

//call back method
void GameLogic::DestroyActorDelegate(IEventPtr pEvent)
{
	std::shared_ptr<Event_DestroyActor> pDestroyActorEvent = std::static_pointer_cast<Event_DestroyActor>(pEvent);
	MyTypes::ActorId id = pDestroyActorEvent->GetActorId();

	std::string eventLog("Destroy Actor event received for actor ID: ");
	eventLog += std::to_string(id);
	ServiceLocator::GetLogService()->VGetLogger()->info(eventLog);
}

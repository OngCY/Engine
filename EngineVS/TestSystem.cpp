#include "TestSystem.h"

TestSystem::~TestSystem()
{
	EventListenerDelegate delegateFunc = fastdelegate::MakeDelegate(this, &TestSystem::DestroyActorDelegate);
	ServiceLocator::GetEventService()->VRemoveListener(delegateFunc, Event_DestroyActor::sk_EventId);
}

//the call back method
void TestSystem::DestroyActorDelegate(IEventPtr pEvent)
{
	std::shared_ptr<Event_DestroyActor> pDestroyActorEvent = std::static_pointer_cast<Event_DestroyActor>(pEvent);
	MyTypes::ActorId id = pDestroyActorEvent->GetActorId();
	//std::cout << "Destroy actor event received. Actor ID: " << id << std::endl;
}

//create a delegate for the call back and register the delegate with the event manager
void TestSystem::Init(void)
{
	EventListenerDelegate delegateListener = fastdelegate::MakeDelegate(this, &TestSystem::DestroyActorDelegate);
	ServiceLocator::GetEventService()->VRegisterListener(delegateListener, Event_DestroyActor::sk_EventId);
}
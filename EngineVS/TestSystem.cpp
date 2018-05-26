#include "TestSystem.h"


void TestEventSystem::TestEventDelegate(IEventPtr pEvent)
{
	std::shared_ptr<Event_DestroyActor> pDestroyActorEvent = std::static_pointer_cast<Event_DestroyActor>(pEvent);
	MyTypes::ActorId id = pDestroyActorEvent->GetId();
}
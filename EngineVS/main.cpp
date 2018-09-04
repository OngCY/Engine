#include "Events.h"
#include "EventManager.h"
#include "ServiceLocator.h"
#include "TestSystem.h"

int main()
{
	IEventManager* eventManager = new EventManager();
	
	//Service Locator pattern for "singleton" major systems such as event handling
	ServiceLocator::Initialise();
	ServiceLocator::SetEventService(eventManager);

	//Test system for event handling
	TestEventSystem* tEventSystem = new TestEventSystem();
	tEventSystem->Init();
	
	//Create the event and fire it immediately. The alternative is to queue the event
	std::shared_ptr<Event_DestroyActor> pDestroyEvent(new Event_DestroyActor(01));
	ServiceLocator::GetEventService()->VTriggerEvent(pDestroyEvent);

	std::getchar();
	return 0;
}
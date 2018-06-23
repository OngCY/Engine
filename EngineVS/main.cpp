#include "Events.h"
#include "EventManager.h"
#include "ServiceLocator.h"

int main()
{
	IEventManager* eventManager = new EventManager();
	ServiceLocator::Initialise();
	ServiceLocator::SetEventService(eventManager);
	
	//Instantiate the event
	std::shared_ptr<Event_DestroyActor> pDestroyEvent(new Event_DestroyActor(01));
	
	//Fire the event immediately. Alternatively, can queue the event
	ServiceLocator::GetEventService()->VTriggerEvent(pDestroyEvent);
	return 0;
}
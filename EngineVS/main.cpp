#include "Events.h"
#include "EventManager.h"
#include "ServiceLocator.h"

int main()
{
	IEventManager* eventManager = new EventManager();
	ServiceLocator::Initialise();
	ServiceLocator::SetEventService(eventManager);
	
	return 0;
}
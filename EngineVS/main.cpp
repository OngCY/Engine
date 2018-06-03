#include "Events.h"
#include "EventManager.h"
#include "ServiceLocator.h"

int main()
{
	IEventManager* eventManager = new EventManager();
	EventManagerLocator::SetService(eventManager);
	return 0;
}
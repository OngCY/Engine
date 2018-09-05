#include "ServiceLocator.h"

//define static member variables in a cpp file
IEventManagerPtr ServiceLocator::eventService;
NullEventManagerPtr ServiceLocator::nullEventService(new NullEventManager());

//to ensure that eventService points to an object and does not return NULL
void ServiceLocator::Initialise()
{
	eventService = nullEventService;
}

IEventManagerPtr ServiceLocator::GetEventService()
{
	return eventService;
}

void ServiceLocator::SetEventService(IEventManagerPtr service)
{
	//ensure that eventService is assigned to an object and does not return NULL
	if (service == NULL)
		eventService = nullEventService;
	else
		eventService = service;
}
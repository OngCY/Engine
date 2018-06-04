#include "ServiceLocator.h"

//define static member variables in a cpp file
IEventManager* ServiceLocator::eventService;
NullEventManager ServiceLocator::nullEventService;

void ServiceLocator::Initialise()
{
	eventService = &nullEventService;
}

IEventManager* ServiceLocator::GetEventService()
{
	return eventService;
}

void ServiceLocator::SetEventService(IEventManager* service)
{
	if (service == NULL)
		eventService = &nullEventService;
	else
		eventService = service;
}

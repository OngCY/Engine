#include "ServiceLocator.h"

//define static member variables in a cpp file
IEventManagerPtr ServiceLocator::eventService;
NullEventManagerPtr ServiceLocator::nullEventService(new NullEventManager());
LogManagerPtr ServiceLocator::logService;
NullLogManagerPtr ServiceLocator::nullLogService(new NullLogManager());

//ensure that eventService points to an object and does not return NULL. Should be called early
void ServiceLocator::Initialise()
{
	eventService = nullEventService;
	logService = nullLogService;
}

////////////////////////////EVENT MANAGER//////////////////////////////////////
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

////////////////////////////LOG MANAGER//////////////////////////////////////
LogManagerPtr ServiceLocator::GetLogService()
{
	return logService;
}

void ServiceLocator::SetLogService(LogManagerPtr service)
{
	//ensure that eventService is assigned to an object and does not return NULL
	if (service == NULL)
		logService = nullLogService;
	else
		logService = service;
}
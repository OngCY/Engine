#include "ServiceLocator.h"

//define static member variables in a cpp file
IEventManagerPtr ServiceLocator::eventService;
NullEventManagerPtr ServiceLocator::nullEventService(new NullEventManager());
LogManagerPtr ServiceLocator::logService;
NullLogManagerPtr ServiceLocator::nullLogService(new NullLogManager());
RenderManagerPtr ServiceLocator::renderService;
NullRenderManagerPtr ServiceLocator::nullRenderService(new NullRenderManager());
ProcessManagerPtr ServiceLocator::processService;
NullProcessManagerPtr ServiceLocator::nullProcessService(new NullProcessManager());

//ensure that the services point to an object and do not return NULL. Should be called early
void ServiceLocator::Initialise()
{
	eventService = nullEventService;
	logService = nullLogService;
	renderService = nullRenderService;
	processService = nullProcessService;
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
	if (service == NULL)
		logService = nullLogService;
	else
		logService = service;
}

////////////////////////////RENDER MANAGER//////////////////////////////////////
RenderManagerPtr ServiceLocator::GetRenderService()
{
	return renderService;
}

void ServiceLocator::SetRenderService(RenderManagerPtr service)
{
	if (service == NULL)
		renderService = nullRenderService;
	else
		renderService = service;
}

////////////////////////////PROCESS MANAGER//////////////////////////////////////
ProcessManagerPtr ServiceLocator::GetProcessService()
{
	return processService;
}

void ServiceLocator::SetProcessService(ProcessManagerPtr service)
{
	if (service == NULL)
		processService = nullProcessService;
	else
		processService = service;
}
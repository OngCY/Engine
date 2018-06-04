#pragma once
#include "EventManager.h"

//Service Locator pattern
//Global point of access to services without coupling clients to the concrete classes
class ServiceLocator
{
public:
	static void Initialise();
	static IEventManager* GetEventService();
	static void SetEventService(IEventManager* service);

private:
	static IEventManager* eventService;
	static NullEventManager nullEventService;
};
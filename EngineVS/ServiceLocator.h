#pragma once
#include "EventManager.h"

typedef std::shared_ptr<IEventManager> IEventManagerPtr;
typedef std::shared_ptr<NullEventManager> NullEventManagerPtr;

//Service Locator pattern
//Global point of access to services without coupling clients to the concrete classes
class ServiceLocator
{
public:
	static void Initialise();
	static IEventManagerPtr GetEventService();
	static void SetEventService(IEventManagerPtr service);

private:
	static IEventManagerPtr eventService;
	static NullEventManagerPtr nullEventService;
};
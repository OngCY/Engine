#pragma once
#include "EventManager.h"

class EventManagerLocator
{
public:
	static IEventManager* GetService();
	static void SetService(IEventManager* service);

private:
	static IEventManager* service_;
	static IEventManager* nullService;
};
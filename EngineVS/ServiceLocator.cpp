#include "ServiceLocator.h"

IEventManager* EventManagerLocator::service_ = NULL;
IEventManager* EventManagerLocator::nullService = new NullEventManager();

IEventManager* EventManagerLocator::GetService()
{
	if (service_ == NULL)
		return nullService;
	else	
		return service_;
}

void EventManagerLocator::SetService(IEventManager* service)
{
	service_ = service;
}

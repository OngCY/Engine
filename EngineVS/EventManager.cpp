#include "EventManager.h"


bool EventManager::VAddListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& eventId) { return true; }
bool EventManager::VRemoveListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& eventId) { return true; }
bool EventManager::VTriggerEvent(const IEventPtr& pEvent) const{return true;}
bool EventManager::VQueueEvent(const IEventPtr& pEvent) const { return true; }
bool EventManager::VAbortEvent(const MyTypes::EventId& id, bool allOfType){ return true; }
bool EventManager::VTickUpdate(unsigned long maxMillis) { return true; }
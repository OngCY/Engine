#pragma once
#include "ThirdParty\FastDelegate.h"
#include "ThirdParty\FastDelegateBind.h"
#include "Events.h"
#include "MyTypes.h"
#include <memory>
#include <list>
#include <map>

typedef std::shared_ptr<IEvent> IEventPtr;
typedef fastdelegate::FastDelegate1<IEventPtr> EventListenerDelegate;
const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

class IEventManager
{
public:

	enum eConstants { kINFINITE = 0xffffffff };

	IEventManager() {}
	virtual ~IEventManager(void) {};
	
	//registers an event id and its corresponding delegate function
	//returns true if successful
	virtual bool VRegisterListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& id) = 0;
	
	//removes a (delegate function+event id) pair from the registry
	//returns false if the pair is not found
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& id) = 0;

	//fires the event now and calls all delegate functions registered for the event
	virtual bool VTriggerEvent(const IEventPtr& pEvent) const = 0;

	//addes the event to the event queue for firing during the next call to VTickUpdate
	virtual bool VQueueEvent(const IEventPtr& pEvent) = 0;

	//removes the next available instance of EventId from the queue
	//if allOfType is true, all events of that id are removed
	//returns true if the event was found and removed
	virtual bool VAbortEvent(const MyTypes::EventId& id, bool allOfType = false) = 0;

	//proceses queued events
	//specify a time limit so that event processing does not take too long
	//return true if all events were processed, false if otherwise
	virtual bool VTickUpdate(unsigned long maxMillis = kINFINITE) = 0;

	//static IEventManager* GetEventManager();
};

class EventManager : public IEventManager
{
	typedef std::list<EventListenerDelegate> EventListenerList;
	typedef std::map<MyTypes::EventId, EventListenerList> EventListenerMap;
	typedef std::list<IEventPtr> EventQueue;

	EventListenerMap m_eventRegistry;
	EventQueue m_eventQueue[EVENTMANAGER_NUM_QUEUES];
	
	//index of active processing queue. Events are added to the other queue
	unsigned int m_activeQueue;

public:
	EventManager() { m_activeQueue = 0; }
	virtual ~EventManager(void) {}

	virtual bool VRegisterListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& eventId);
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& eventId);
	virtual bool VTriggerEvent(const IEventPtr& pEvent) const;
	virtual bool VQueueEvent(const IEventPtr& pEvent);
	virtual bool VAbortEvent(const MyTypes::EventId& id, bool allOfType = false);
	virtual bool VTickUpdate(unsigned long maxMillis = kINFINITE);
};

class NullEventManager : public IEventManager
{
public:
	NullEventManager() {}
	virtual ~NullEventManager() {}

	virtual bool VRegisterListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& eventId) { return false; }
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& eventId) { return false; }
	virtual bool VTriggerEvent(const IEventPtr& pEvent) const { return false; }
	virtual bool VQueueEvent(const IEventPtr& pEvent) { return false; }
	virtual bool VAbortEvent(const MyTypes::EventId& id, bool allOfType = false) { return false; }
	virtual bool VTickUpdate(unsigned long maxMillis = kINFINITE) { return false; }
};

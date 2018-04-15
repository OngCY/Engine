#include "EventManager.h"
#include <iostream>


bool EventManager::VRegisterEvent(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& eventId) 
{ 
	//check if the event already exists in the registry
	EventListenerMap::iterator it;
	it = m_eventRegistry.find(eventId);

	if (it != m_eventRegistry.end())
	{
		std::cout<< "VRegisterEvent: Event " << eventId << " already exists" << std::endl;
		return false;
	}
	
	EventListenerList delegateList;
	delegateList.push_back(eventDelegate);

	m_eventRegistry.insert(std::make_pair(eventId, delegateList));
	
	return true; 
}

bool EventManager::VAddListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& eventId)
{
	//check if the event exists first
	if (m_eventRegistry.find(eventId) == m_eventRegistry.end())
	{
		std::cout << "VAddListener: The event does not exist. Add the event first" << std::endl;
		return false;
	}
	
	//then check if the listener delegate already exists for the event
	EventListenerList& eventListenerList = m_eventRegistry[eventId];
	for (auto it : eventListenerList)
	{
		if ((it) == eventDelegate)
		{
			std::cout << "VAddListener: The event listener delegate already exists" << std::endl;
			return false;
		}
	}

	eventListenerList.push_back(eventDelegate);

	return true;
}

bool EventManager::VRemoveListener(const EventListenerDelegate& eventDelegate, const MyTypes::EventId& eventId)
{ 
	bool result = false;

	//check that the event exists first
	auto it = m_eventRegistry.find(eventId);
	if (it == m_eventRegistry.end())
	{
		std::cout << "VRemoveListener: The event is not found" << std::endl;
		return false;
	}

	//then search through the list of event listener delegates and remove the matching one
	EventListenerList& eventListenerList = it->second;
	for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
	{
		if ((*it) == eventDelegate)
		{
			eventListenerList.erase(it);
			result = true;
		}
	}

	return result; 
}

bool EventManager::VTriggerEvent(const IEventPtr& pEvent) const
{
	bool triggered = false;

	//find the event in the registry and trigger the delegates registered with the event
	auto eventIt = m_eventRegistry.find(pEvent->VGetEventID());
	if (eventIt != m_eventRegistry.end())
	{
		EventListenerList list = eventIt->second;
		for (EventListenerList::const_iterator it = list.begin(); it != list.end(); ++it)
		{
			EventListenerDelegate listener = (*it);
			listener(pEvent);
			triggered = true;
		}
	}
	
	return true;
}

bool EventManager::VQueueEvent(const IEventPtr& pEvent)  
{
	_ASSERT(m_activeQueue >= 0);
	_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

	//add an event to the queue if the event is registered and has delegate call backs
	auto eventIt = m_eventRegistry.find(pEvent->VGetEventID());
	if (eventIt != m_eventRegistry.end() && !eventIt->second.empty())
	{
		m_eventQueue[m_activeQueue].push_back(pEvent);
		return true;
	}
	else
		return false; 
}
bool EventManager::VAbortEvent(const MyTypes::EventId& id, bool allOfType){ return true; }
bool EventManager::VTickUpdate(unsigned long maxMillis) { return true; }
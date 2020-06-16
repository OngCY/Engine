#include "EventManager.h"
#include <iostream>
#include <Windows.h>

//define static member variables in a cpp file
const MyTypes::EventId Event_DestroyActor::sk_EventId = 0x50dbd404;
const MyTypes::EventId Event_TranslatePlayer::sk_EventId = 0x50dbd405;

bool EventManager::VRegisterListener(const EventListenerDelegate_t& eventDelegate, const MyTypes::EventId& eventId)
{	
	//find or create the event-listener
	//If k matches the key of an element in the container, the function returns a reference to its mapped value.
	//If k does not match the key of any element in the container, the function inserts a new element with that key and returns a reference to its mapped value
	EventListenerList_t& eventListenerList = m_eventRegistry[eventId];
	for (EventListenerList_t::iterator listenerIt = eventListenerList.begin(); listenerIt != eventListenerList.end(); ++listenerIt)
	{
		//disallow duplicate event listener delegates for an event
		if ((*listenerIt) == eventDelegate)
		{
			std::cout << "VAddListener: The event listener delegate already exists" << std::endl;
			return false;
		}
	}

	eventListenerList.push_back(eventDelegate);

	return true;
}

bool EventManager::VRemoveListener(const EventListenerDelegate_t& eventDelegate, const MyTypes::EventId& eventId)
{ 
	bool removed = false;

	//check that the event exists first
	EventListenerMap_t::iterator eventIt = m_eventRegistry.find(eventId);
	if (eventIt == m_eventRegistry.end())
	{
		std::cout << "VRemoveListener: The event is not found" << std::endl;
		return false;
	}

	//then search through the list of event listener delegates and remove the matching one
	//there should only be 1 match. Each event listener delegate should be unique for an event
	EventListenerList_t& eventListenerList = eventIt->second;
	for (EventListenerList_t::iterator listenerIt = eventListenerList.begin(); listenerIt != eventListenerList.end(); ++listenerIt)
	{
		if ((*listenerIt) == eventDelegate)
		{
			eventListenerList.erase(listenerIt); //note: the iterator to the erased element is invalidated
			removed = true;
			break;
		}
	}

	return removed;
}

bool EventManager::VTriggerEvent(const IEventPtr_t& pEvent) const
{
	bool triggered = false;

	//find the event in the registry and trigger the delegates registered with the event
	EventListenerMap_t::const_iterator eventIt = m_eventRegistry.find(pEvent->VGetEventId());
	if (eventIt != m_eventRegistry.end())
	{
		const EventListenerList_t& listenerList = eventIt->second;
		for (EventListenerList_t::const_iterator listenerIt = listenerList.begin(); listenerIt != listenerList.end(); ++listenerIt)
		{
			EventListenerDelegate_t listener = (*listenerIt);
			listener(pEvent);
			triggered = true;
		}
	}
	
	return true;
}

bool EventManager::VQueueEvent(const IEventPtr_t& pEvent)  
{
	_ASSERT(m_activeQueue >= 0);
	_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

	//add an event to the queue if the event is registered and has delegate call backs
	EventListenerMap_t::const_iterator eventIt = m_eventRegistry.find(pEvent->VGetEventId());
	if (eventIt != m_eventRegistry.end() && !eventIt->second.empty())
	{
		m_eventQueue[m_activeQueue].push_back(pEvent);
		return true;
	}
	else
		return false; 
}

bool EventManager::VAbortEvent(const MyTypes::EventId& id, bool abortAllOfType)
{
	_ASSERT(m_activeQueue >= 0);
	_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

	bool abortSuccess = false;

	//check that the event exists in the registry first
	EventListenerMap_t::const_iterator registryIt = m_eventRegistry.find(id);

	if (registryIt != m_eventRegistry.end())
	{
		//get the current active processed queue
		EventQueue_t& eventQ = m_eventQueue[m_activeQueue];

		//search through the queue for the event and remove it from the queue
		for (EventQueue_t::iterator eventQIt = eventQ.begin(); eventQIt != eventQ.end(); ++eventQIt)
		{
			//create a temp iterator and use it for the rest of the loop
			//because removing an item from the std list will invalidate the iterator
			EventQueue_t::iterator tempIt = eventQIt;

			if ((*tempIt)->VGetEventId() == id)
			{
				eventQ.erase(tempIt);
				abortSuccess = true;

				//exit the loop if there is no need to remove all events of the same type
				if (!abortAllOfType) 
					break;
			}
		}
	}

	return abortSuccess; 
}

bool EventManager::VTickUpdate(unsigned long deltaMillis) 
{
	unsigned long currentMS = GetTickCount();
	unsigned long endMS;

	if (deltaMillis == IEventManager::kINFINITE)
		endMS = IEventManager::kINFINITE;
	else
		endMS = currentMS + deltaMillis; //time for the queue processing to end

	int currentQueue = m_activeQueue;

	//process the current active event queue
	while (!m_eventQueue[currentQueue].empty())
	{
		//process the event which is the first in the queue and remove it
		IEventPtr_t pEvent = m_eventQueue[currentQueue].front();
		m_eventQueue[currentQueue].pop_front();

		//execute all delegate functions registered using the event ID
		EventListenerMap_t::const_iterator registryIt = m_eventRegistry.find(pEvent->VGetEventId());
		if (registryIt != m_eventRegistry.end())
		{
			const EventListenerList_t& listenerList = registryIt->second;

			for (EventListenerList_t::const_iterator listenerIt = listenerList.begin(); listenerIt != listenerList.end(); ++listenerIt)
			{
				EventListenerDelegate_t listenerDelegate = (*listenerIt);
				listenerDelegate(pEvent);
			}
		}

		//stop processing the event queue if time has run out
		currentMS = GetTickCount();
		if (deltaMillis != IEventManager::kINFINITE && currentMS >= endMS)
		{
			std::cout << "VTickUpdate: Exceeded time for event queue processing" << std::endl;
			break;
		}
	}

	//toggle the next active queue and clear it
	m_activeQueue = (m_activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
	m_eventQueue[m_activeQueue].clear();

	//if there are events left unprocessed, move the events to the next active queue
	//preserve the event sequence (older events to be processed first in the next active queue)
	bool isFlushed = m_eventQueue[currentQueue].empty();

	if (!isFlushed)
	{
		while (!m_eventQueue[currentQueue].empty())
		{
			IEventPtr_t pEvent = m_eventQueue[currentQueue].back();
			m_eventQueue[currentQueue].pop_back();
			m_eventQueue[m_activeQueue].push_front(pEvent);
		}
	}

	return isFlushed;
}
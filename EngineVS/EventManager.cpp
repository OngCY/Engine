#include "EventManager.h"
#include <iostream>
#include <Windows.h>


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

bool EventManager::VAbortEvent(const MyTypes::EventId& id, bool allOfType)
{
	_ASSERT(m_activeQueue >= 0);
	_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

	bool aborted = false;

	//check that the event exists in the registry first
	EventListenerMap::iterator registryIt = m_eventRegistry.find(id);

	if (registryIt != m_eventRegistry.end())
	{
		EventQueue& queue = m_eventQueue[m_activeQueue];

		//search through the queue for the event and remove it from the queue
		for (EventQueue::iterator eventIt = queue.begin(); eventIt != queue.end(); ++eventIt)
		{
			//create a temp iterator and use it for the rest of the loop
			//because removing an item from the std list will invalidate the iterator
			EventQueue::iterator tempIt = eventIt;

			if ((*tempIt)->VGetEventID() == id)
			{
				queue.erase(tempIt);
				aborted = true;

				//exit the loop if there is no need to remove all events of the same type
				if (!allOfType) 
					break;
			}
		}
	}

	return aborted; 
}

bool EventManager::VTickUpdate(unsigned long deltaMillis) 
{
	unsigned long currentMS = GetTickCount();
	unsigned long endMS;

	if (deltaMillis == IEventManager::kINFINITE)
		endMS = IEventManager::kINFINITE;
	else
		endMS = currentMS + deltaMillis;

	int currentQueue = m_activeQueue;

	//process the current active event queue
	while (!m_eventQueue[currentQueue].empty())
	{
		//process the event which is the first in the queue
		IEventPtr pEvent = m_eventQueue[currentQueue].front();
		m_eventQueue[currentQueue].pop_front();

		//find all delegate functions registered using the event ID
		const MyTypes::EventId& eventID = pEvent->VGetEventID();
		auto eventIt = m_eventRegistry.find(eventID);
		if (eventIt != m_eventRegistry.end())
		{
			const EventListenerList& listenerList = eventIt->second;

			for (auto delegateIt = listenerList.begin(); delegateIt != listenerList.end(); ++delegateIt)
			{
				EventListenerDelegate listenerDelegate = (*delegateIt);
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

	//set the next active queue and clear it
	m_activeQueue = (m_activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
	m_eventQueue[m_activeQueue].clear();

	//if there are events left unprocessed, shift the events to the next active queue
	//preserve the event sequence (older events to be processed first in the next active queue)
	bool isFlushed = m_eventQueue[currentQueue].empty();

	if (!isFlushed)
	{
		while (!m_eventQueue[currentQueue].empty())
		{
			IEventPtr pEvent = m_eventQueue[currentQueue].back();
			m_eventQueue[currentQueue].pop_back();
			m_eventQueue[m_activeQueue].push_front(pEvent);
		}
	}

	return isFlushed;
}
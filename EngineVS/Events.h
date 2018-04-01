#pragma once
#include <sstream>

typedef unsigned long EventId;
typedef unsigned int ActorId;

class IEvent
{
public:
	virtual const EventId& VGetEventID(void) const = 0;
	virtual float VGetTimeStamp(void) const = 0;
	virtual void VSerialise(std::ostringstream& out) const = 0;
	virtual const char* GetName(void) const = 0;
	//virtual IEventDataPtr VCopy(void) const = 0;
};

class BaseEvent : public IEvent
{
	const float m_timeStamp;

public:
	explicit BaseEvent(const float timeStamp = 0.0f) : m_timeStamp(timeStamp) {}
	virtual ~BaseEvent(void) {}

	virtual const EventId& VGetEventId(void) const = 0;
	float VGetTimeStamp(void) const { return m_timeStamp; }
	virtual void VSerialise(std::ostringstream &out) const {}
};

class Event_DestroyActor : public BaseEvent
{
	ActorId m_id;

public:
	static const EventId sk_EventId;

	explicit Event_DestroyActor(ActorId id) :m_id(id) {}
	explicit Event_DestroyActor(std::istringstream &in) { in >> m_id; }

	virtual const EventId& VGetEventType(void) const
	{
		return sk_EventId;
	}

	virtual void VSerialize(std::ostringstream &out) const
	{
		out << m_id;
	}

	virtual const char* GetName(void)
	{
		return "Event_DestroyActor";
	}

	ActorId GetId(void) const
	{
		return m_id;
	}
};
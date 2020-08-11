#pragma once
#include "MyTypes.h"
#include <sstream>

class IEvent
{
public:
	virtual const MyTypes::EventId& VGetEventId(void) const = 0;
	virtual float VGetTimeStamp(void) const = 0;
	virtual void VSerialise(std::ostringstream& out) const = 0;
	virtual const char* GetName(void) const = 0;
};

class BaseEvent : public IEvent
{
	const float m_timeStamp;

public:
	explicit BaseEvent(const float timeStamp = 0.0f) : m_timeStamp(timeStamp) {}
	virtual ~BaseEvent(void) {}

	virtual const MyTypes::EventId& VGetEventId(void) const = 0;
	float VGetTimeStamp(void) const { return m_timeStamp; }
	virtual void VSerialise(std::ostringstream &out) const {}
	virtual const char* GetName(void) const { return "BaseEvent"; }
};

class Event_TranslatePlayer : public BaseEvent
{
	MyTypes::ActorId m_id;
	MyTypes::TranslateType m_type;

public:
	static const MyTypes::EventId sk_EventId;
	
	explicit Event_TranslatePlayer(MyTypes::ActorId id, MyTypes::TranslateType type) :m_id(id), m_type(type) {}
	
	virtual const MyTypes::EventId& VGetEventId(void) const
	{
		return sk_EventId;
	}

	virtual const char* GetName(void) const
	{
		return "Event_TranslatePlayer";
	}

	MyTypes::ActorId GetActorId(void) const
	{
		return m_id;
	}

	MyTypes::TranslateType GetTranslateType(void) const
	{
		return m_type;
	}
};
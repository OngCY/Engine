#pragma once
#include "MyTypes.h"
#include <sstream>


//////////////////////EVENT INTERFACE/////////////////////
class IEvent
{
public:
	virtual const MyTypes::EventId& VGetEventId(void) const = 0;
	virtual float VGetTimeStamp(void) const = 0;
	virtual void VSerialise(std::ostringstream& out) const = 0;
	virtual const char* GetName(void) const = 0;
	//virtual IEventPtr VCopy(void) const = 0;
};

//////////////////////BASE EVENT/////////////////////
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

//////////////////////TRANSLATE ACTOR EVENT/////////////////////
class Event_TranslateActor : public BaseEvent
{
	MyTypes::ActorId m_id;
	MyTypes::TranslateType m_type;

public:
	static const MyTypes::EventId sk_EventId;

	explicit Event_TranslateActor(MyTypes::ActorId id, MyTypes::TranslateType type) :m_id(id), m_type(type) {}
	
	virtual const MyTypes::EventId& VGetEventId(void) const
	{
		return sk_EventId;
	}

	virtual const char* GetName(void) const
	{
		return "Event_TranslateActor";
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

//////////////////////DESTROY ACTOR EVENT/////////////////////
class Event_DestroyActor : public BaseEvent
{
	MyTypes::ActorId m_id;

public:
	static const MyTypes::EventId sk_EventId;

	explicit Event_DestroyActor(MyTypes::ActorId id) :m_id(id) {}
	explicit Event_DestroyActor(std::istringstream &in) { in >> m_id; }

	virtual const MyTypes::EventId& VGetEventId(void) const
	{
		return sk_EventId;
	}

	virtual void VSerialize(std::ostringstream &out) const
	{
		out << m_id;
	}

	virtual const char* GetName(void) const
	{
		return "Event_DestroyActor";
	}

	MyTypes::ActorId GetActorId(void) const
	{
		return m_id;
	}
};
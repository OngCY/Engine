#pragma once
#include "EventManager.h"
#include "LogManager.h"

typedef std::shared_ptr<IEventManager> IEventManagerPtr;
typedef std::shared_ptr<NullEventManager> NullEventManagerPtr;
typedef std::shared_ptr<LogManager> LogManagerPtr;
typedef std::shared_ptr<NullLogManager> NullLogManagerPtr;

//Service Locator pattern
//Global point of access to services without coupling clients to the concrete classes
class ServiceLocator
{
public:
	static void Initialise();
	static IEventManagerPtr GetEventService();
	static void SetEventService(IEventManagerPtr service);
	static LogManagerPtr GetLogService();
	static void SetLogService(LogManagerPtr service);

private:
	static IEventManagerPtr eventService;
	static NullEventManagerPtr nullEventService;
	static LogManagerPtr logService;
	static NullLogManagerPtr nullLogService;
};
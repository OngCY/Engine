#pragma once
#include "EventManager.h"
#include "LogManager.h"
#include "RenderManager.h"
#include "Process.h"

typedef std::shared_ptr<IEventManager> IEventManagerPtr;
typedef std::shared_ptr<NullEventManager> NullEventManagerPtr;
typedef std::shared_ptr<LogManager> LogManagerPtr;
typedef std::shared_ptr<NullLogManager> NullLogManagerPtr;
typedef std::shared_ptr<RenderManager> RenderManagerPtr;
typedef std::shared_ptr<NullRenderManager> NullRenderManagerPtr;
typedef std::shared_ptr<ProcessManager> ProcessManagerPtr;
typedef std::shared_ptr<NullProcessManager> NullProcessManagerPtr;

//////////////////////SERVICE LOCATOR PATTERN/////////////////////
//Global point of access to services without coupling clients to the concrete classes
class ServiceLocator
{
public:
	static void Initialise();

	static IEventManagerPtr GetEventService();
	static void SetEventService(IEventManagerPtr service);
	static LogManagerPtr GetLogService();
	static void SetLogService(LogManagerPtr service);
	static RenderManagerPtr GetRenderService();
	static void SetRenderService(RenderManagerPtr service);
	static ProcessManagerPtr GetProcessService();
	static void SetProcessService(ProcessManagerPtr service);

private:
	static IEventManagerPtr eventService;
	static NullEventManagerPtr nullEventService;
	static LogManagerPtr logService;
	static NullLogManagerPtr nullLogService;
	static RenderManagerPtr renderService;
	static NullRenderManagerPtr nullRenderService;
	static ProcessManagerPtr processService;
	static NullProcessManagerPtr nullProcessService;
};
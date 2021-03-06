#include "ServiceLocator.h"
#include "GameLogic.h"
#include "ThirdParty\json.hpp"
#include "Actors.h"
#include "MovementController.h"
#include <fstream>
#include <chrono>
#include <string>

using json = nlohmann::json;
using namespace std::chrono;

int main()
{
	std::shared_ptr<LogManager> pLogManager(new LogManager()); pLogManager->VInitLogging();
	std::shared_ptr<RenderManager> pRenderManager(new RenderManager()); pRenderManager->VInitRenderer();
	std::shared_ptr<ProcessManager> pProcessManager(new ProcessManager());
	std::shared_ptr<IEventManager> pEventManager(new EventManager());

	ServiceLocator::Initialise();
	ServiceLocator::SetLogService(pLogManager);
	ServiceLocator::SetRenderService(pRenderManager);
	ServiceLocator::SetProcessService(pProcessManager);
	ServiceLocator::SetEventService(pEventManager);
	MovementController* pMovementCtrl = new MovementController();
	ServiceLocator::GetRenderService()->VSetEventReceiver(pMovementCtrl);

	GameLogic *gameLogic = new GameLogic();
	gameLogic->Init();

	system_clock::time_point startTime = system_clock::now();
	int64_t lag = 0;
	const int64_t MS_PER_UPDATE = 16; //60 fps

	/*********DELAY PROCESS TEST********/
	StrongProcessPtr delayProcessPtr(new DelayProcess(5000));
	pProcessManager->AttachProcess(delayProcessPtr);
	
	while (ServiceLocator::GetRenderService()->VRunRenderer())
	{
		system_clock::time_point currentTime = system_clock::now();
		int64_t elapsed = duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
		startTime = currentTime;
		lag += elapsed;
		
		//game update
		while (lag >= MS_PER_UPDATE)
		{
			//update methods
			ServiceLocator::GetEventService()->VTickUpdate(MS_PER_UPDATE);
			ServiceLocator::GetProcessService()->UpdateAllProcesses(MS_PER_UPDATE);
			gameLogic->Update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}

		//render
		ServiceLocator::GetRenderService()->VStartScene();
		ServiceLocator::GetRenderService()->VDrawSceneNodes();
		ServiceLocator::GetRenderService()->VDrawUIElements();
		ServiceLocator::GetRenderService()->VRenderToScreen();
	}	

	//delete actorFactory;
	gameLogic->Cleanup();
	delete gameLogic; gameLogic = nullptr;

	ServiceLocator::GetRenderService()->VCloseRenderer();
	ServiceLocator::GetLogService()->VCloseLogging();

	return 0;
}

/////////////Previous Test Code/////////////////
/*********EVENT TEST********/
/*
std::shared_ptr<IEventManager> pEventManager(new EventManager());

//Service Locator pattern. Call the Initialise method early
ServiceLocator::Initialise();
ServiceLocator::SetEventService(pEventManager);

//Test system for event handling
TestSystem* tEventSystem = new TestSystem();
tEventSystem->Init();

//Create the event and fire it immediately. The alternative is to queue the event
std::shared_ptr<Event_DestroyActor> pDestroyEvent(new Event_DestroyActor(01));
ServiceLocator::GetEventService()->VTriggerEvent(pDestroyEvent);

delete tEventSystem;
*/

/********NLOHMAN JSON LIBRARY TEST******/
/*
json j;
j = {
{"pi", 3.142 },
{"name", "ong"},
{"object",
{
{"currency", "usd"},
{"value", 42}
}
}
};
std::cout << j << std::endl;

//nlohmann json library: json file parsing
std::ifstream ifs("C:\\engine\\Engine\\EngineVS\\configuration\\components.json");
json jstream = json::parse(ifs);
std::cout << jstream.at("NumComponents") << std::endl;
std::cout << jstream["PhysicsComponent"]["material"] << std::endl;
std::cout << jstream["PhysicsComponent"]["mass"] << std::endl;

//nlohmann json library: array iteration
std::vector<std::string> componentVector = jstream["Components"];
for (auto comp : componentVector)
std::cout << comp << std::endl;

//nlohmann json library: sub object access
json jsubstream = jstream["PhysicsComponent"];
std::cout << jsubstream["material"] << std::endl;
*/
/********GAME ACTORS TEST******/

/********LOG TEST******/
/*
auto my_logger = spdlog::basic_logger_mt("basic_logger", "basic.txt");
my_logger->info("test log");
spdlog::drop_all();
*/
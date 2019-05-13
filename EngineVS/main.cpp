#include "Events.h"
#include "EventManager.h"
#include "LogManager.h"
#include "ServiceLocator.h"
#include "TestSystem.h"
#include "ThirdParty\json.hpp"
#include "Actors.h"
#include <fstream>
#include <chrono>
#include <irrlicht.h>

using json = nlohmann::json;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std::chrono;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	/*IrrlichtDevice *device = createDevice(video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16, false, false, false, 0);
	
	if (!device) return 1;

	device->setWindowCaption(L"Test Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! Process Test", rect<s32>(10, 10, 260, 22), true);
	*/

	std::shared_ptr<LogManager> pLogManager(new LogManager());
	pLogManager->VInitLogging();

	ServiceLocator::Initialise();
	ServiceLocator::SetLogService(pLogManager);
	ServiceLocator::GetLogService()->VGetLogger()->info("Welcome to SPD Rotating Log");
	ServiceLocator::GetLogService()->VCloseLogging();

	system_clock::time_point startTime = system_clock::now();
	int64_t lag = 0;
	const int64_t MS_PER_UPDATE = 16; //60 fps	
	
	/*
	while (device->run())
	{
		system_clock::time_point currentTime = system_clock::now();
		int64_t elapsed = duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
		startTime = currentTime;
		lag += elapsed;

		//processInput()

		//game update
		while (lag >= MS_PER_UPDATE)
		{
			//update()
			lag -= MS_PER_UPDATE;
		}

		//render
		driver->beginScene(true, true, SColor(255, 100, 101, 140));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
	}*/
	
	//device->drop();

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
/*
ActorFactory* actorFactory = new ActorFactory();

StrongActorPtr greenHerbActor(actorFactory->CreateActor("C:\\Git\\Engine\\EngineVS\\configuration\\components_GreenHerb.json"));
std::weak_ptr<HealthPickUp> wHealthPickUp(greenHerbActor->GetComponent<HealthPickUp>(COMPONENTS::PICKUP_HEALTH));
std::shared_ptr<HealthPickUp> sHealthPickUp = wHealthPickUp.lock(); //lock returns a shard_ptr
int boost = 0;
if (sHealthPickUp)
boost = sHealthPickUp->VGetHealthBoost();
else
std::cout << "HealthPickUp shard pointer not valid" << std::endl;

StrongActorPtr npcPartnerActor(actorFactory->CreateActor("C:\\Git\\Engine\\EngineVS\\configuration\\components_NPCPartner.json"));
std::weak_ptr<HealthLifeComponent> wHealthLife(npcPartnerActor->GetComponent<HealthLifeComponent>(COMPONENTS::HEALTH_LIFE));
std::shared_ptr<HealthLifeComponent> sHealthLife = wHealthLife.lock(); //lock returns a shard_ptr
if (sHealthLife)
sHealthLife->VUpdateHealth(boost);
else
std::cout << "HealthLifeComponet shared pointer not valid" << std::endl;

std::getchar();

delete actorFactory;
*/
/********LOG TEST******/
/*
auto my_logger = spdlog::basic_logger_mt("basic_logger", "basic.txt");
my_logger->info("test log");
spdlog::drop_all();
*/
#include "Events.h"
#include "EventManager.h"
#include "ServiceLocator.h"
#include "TestSystem.h"
#include "ThirdParty\json.hpp"
#include <fstream>

using json = nlohmann::json;

int main()
{
	std::shared_ptr<IEventManager> pEventManager(new EventManager());

	//Service Locator pattern. Call the Initialise method early
	ServiceLocator::Initialise();
	ServiceLocator::SetEventService(pEventManager);

	//Test system for event handling
	TestEventSystem* tEventSystem = new TestEventSystem();
	tEventSystem->Init();

	//Create the event and fire it immediately. The alternative is to queue the event
	std::shared_ptr<Event_DestroyActor> pDestroyEvent(new Event_DestroyActor(01));
	ServiceLocator::GetEventService()->VTriggerEvent(pDestroyEvent);

	delete tEventSystem;

	//testing the nlohmann json library: literal json string
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

	//testing the nlohmann json library: json file parsing
	std::ifstream ifs("C:\\engine\\Engine\\EngineVS\\configuration\\components.json");
	json jstream = json::parse(ifs);
	std::cout << jstream.at("NumComponents") << std::endl;
	std::cout << jstream["PhysicsComponent"]["material"] << std::endl;
	std::cout << jstream["PhysicsComponent"]["mass"] << std::endl;

	std::getchar();

	return 0;
}
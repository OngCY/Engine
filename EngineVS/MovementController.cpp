#include "MovementController.h"

bool MovementController::OnEvent(const irr::SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		m_keyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		ProcessKeyInput();
	}

	return false;
}

void MovementController::ProcessKeyInput()
{
	if (m_keyDown[irr::KEY_KEY_W]) //forward movement
	{		
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::FORWARD));
		ServiceLocator::GetEventService()->VTriggerEvent(pTranslateEvent);	
	}

	if (m_keyDown[irr::KEY_KEY_S]) //backward
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::BACKWARD));
		ServiceLocator::GetEventService()->VTriggerEvent(pTranslateEvent);
	}

	if (m_keyDown[irr::KEY_KEY_A]) //left
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::LEFT));
		ServiceLocator::GetEventService()->VTriggerEvent(pTranslateEvent);
	}

	if (m_keyDown[irr::KEY_KEY_D]) //right
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::RIGHT));
		ServiceLocator::GetEventService()->VTriggerEvent(pTranslateEvent);
	}
}
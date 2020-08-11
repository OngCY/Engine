#include "MovementController.h"

bool MovementController::OnEvent(const irr::SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		if (event.KeyInput.PressedDown)
			ProcessKeyDown(event.KeyInput.Key);
		else
			ProcessKeyUp(event.KeyInput.Key);
	}

	return false;
}

void MovementController::ProcessKeyDown(irr::EKEY_CODE key)
{
	switch (key)
	{
	case irr::KEY_KEY_W:
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::FORWARD));
		ServiceLocator::GetEventService()->VQueueEvent(pTranslateEvent);
		break;
	}

	case irr::KEY_KEY_A:
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::LEFT));
		ServiceLocator::GetEventService()->VQueueEvent(pTranslateEvent);
		break;
	}
	case irr::KEY_KEY_S:
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::BACKWARD));
		ServiceLocator::GetEventService()->VQueueEvent(pTranslateEvent);
		break;
	}
	case irr::KEY_KEY_D:
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::RIGHT));
		ServiceLocator::GetEventService()->VQueueEvent(pTranslateEvent);
		break;
	}
	}
}

void MovementController::ProcessKeyUp(irr::EKEY_CODE key)
{
	std::shared_ptr<Event_TranslatePlayer> pStopPlayerEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::STATIONARY));
	ServiceLocator::GetEventService()->VQueueEvent(pStopPlayerEvent);
}

/*
	if (m_keyDown[irr::KEY_KEY_W]) //forward movement
	{		
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::FORWARD));
		ServiceLocator::GetEventService()->VQueueEvent(pTranslateEvent);
		m_keyDown[irr::KEY_KEY_W] = false;
	}

	if (m_keyDown[irr::KEY_KEY_S]) //backward
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::BACKWARD));
		ServiceLocator::GetEventService()->VQueueEvent(pTranslateEvent);
		m_keyDown[irr::KEY_KEY_S] = false;
	}

	if (m_keyDown[irr::KEY_KEY_A]) //left
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::LEFT));
		ServiceLocator::GetEventService()->VQueueEvent(pTranslateEvent);
		m_keyDown[irr::KEY_KEY_A] = false;
	}

	if (m_keyDown[irr::KEY_KEY_D]) //right
	{
		std::shared_ptr<Event_TranslatePlayer> pTranslateEvent(new Event_TranslatePlayer(ACTORID::PLAYER, TRANSLATION::RIGHT));
		ServiceLocator::GetEventService()->VQueueEvent(pTranslateEvent);
		m_keyDown[irr::KEY_KEY_D] = false;
	}
}
*/
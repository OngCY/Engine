#include "MovementController.h"

bool MovementController::OnEvent(const irr::SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		m_keyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
		case irr::EMIE_LMOUSE_PRESSED_DOWN:
			m_mouseState.leftButtonDown = true;
			break;

		case irr::EMIE_LMOUSE_LEFT_UP:
			m_mouseState.leftButtonDown = false;
			break;

		case irr::EMIE_MOUSE_MOVED:
			if ((m_mouseState.lastPosition.X != event.MouseInput.X) ||
				(m_mouseState.lastPosition.Y != event.MouseInput.Y))
			{
				m_targetYaw = m_targetYaw + (m_mouseState.lastPosition.X - event.MouseInput.X);
				m_targetPitch = m_targetPitch + (m_mouseState.lastPosition.Y - event.MouseInput.Y);
				m_mouseState.lastPosition.X = event.MouseInput.X;
				m_mouseState.lastPosition.Y = event.MouseInput.Y;
			}
			return true;

		default:
			break; //mouse wheel not used
		}
	}

	return false;
}

void MovementController::VOnUpdate(const int deltaMilliseconds)
{
	if (m_keyDown[irr::KEY_KEY_W]) //forward movement
	{		
		std::shared_ptr<Event_TranslateActor> pTranslateEvent(new Event_TranslateActor(01,TRANSLATION::FORWARD));
		//ServiceLocator::GetEventService()->VTriggerEvent(pTranslateEvent);	
	}

	if (m_keyDown[irr::KEY_KEY_S]) //backward
	{
		//std::shared_ptr<Event_TranslateActor> pTranslateEvent(new Event_TranslateActor(01, TRANSLATION::BACKWARD));
		//ServiceLocator::GetEventService()->VTriggerEvent(pTranslateEvent);
	}

	if (m_keyDown[irr::KEY_KEY_A]) //left
	{
		//std::shared_ptr<Event_TranslateActor> pTranslateEvent(new Event_TranslateActor(01, TRANSLATION::LEFT));
		//ServiceLocator::GetEventService()->VTriggerEvent(pTranslateEvent);	
	}

	if (m_keyDown[irr::KEY_KEY_D]) //right
	{
		//std::shared_ptr<Event_TranslateActor> pTranslateEvent(new Event_TranslateActor(01, TRANSLATION::RIGHT));
		//ServiceLocator::GetEventService()->VTriggerEvent(pTranslateEvent);
	}
}
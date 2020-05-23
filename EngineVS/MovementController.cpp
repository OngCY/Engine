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
		irr::core::vector3df translationVector = irr::core::vector3df(0.0, 0.0, 1.0);
		irr::core::matrix4 transformMatrix;
		transformMatrix.setTranslation(translationVector);

		std::shared_ptr<Event_TransformActor> pTransformEvent(new Event_TransformActor(1, transformMatrix));
		ServiceLocator::GetEventService()->VTriggerEvent(pTransformEvent);	
	}

	if (m_keyDown[irr::KEY_KEY_S]) //backward
	{
		irr::core::vector3df translationVector = irr::core::vector3df(0.0, 0.0, -1.0);
		irr::core::matrix4 transformMatrix;
		transformMatrix.setTranslation(translationVector);

		std::shared_ptr<Event_TransformActor> pTransformEvent(new Event_TransformActor(1, transformMatrix));
		ServiceLocator::GetEventService()->VTriggerEvent(pTransformEvent);
	}

	if (m_keyDown[irr::KEY_KEY_A]) //left
	{
		irr::core::vector3df translationVector = irr::core::vector3df(-1.0, 0.0, 0.0);
		irr::core::matrix4 transformMatrix;
		transformMatrix.setTranslation(translationVector);

		std::shared_ptr<Event_TransformActor> pTransformEvent(new Event_TransformActor(1, transformMatrix));
		ServiceLocator::GetEventService()->VTriggerEvent(pTransformEvent);
	}

	if (m_keyDown[irr::KEY_KEY_D]) //right
	{
		irr::core::vector3df translationVector = irr::core::vector3df(1.0, 0.0, 0.0);
		irr::core::matrix4 transformMatrix;
		transformMatrix.setTranslation(translationVector);

		std::shared_ptr<Event_TransformActor> pTransformEvent(new Event_TransformActor(1, transformMatrix));
		ServiceLocator::GetEventService()->VTriggerEvent(pTransformEvent);
	}
}
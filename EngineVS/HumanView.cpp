#include "HumanView.h"

bool HumanView::VOnEvent(const SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			m_mouseState.leftButtonDown = true;
			break;

		case EMIE_LMOUSE_LEFT_UP:
			m_mouseState.leftButtonDown = false;
			break;

		case EMIE_MOUSE_MOVED:
			m_mouseState.Position.X = event.MouseInput.X;
			m_mouseState.Position.Y = event.MouseInput.Y;
			break;

		default:
			break; //mouse wheel not used
		}
	}

	return false;
}
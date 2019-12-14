#include "Input.h"

InputEventReceiver::InputEventReceiver()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

bool InputEventReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			MouseState.leftButtonDown = true;
			break;

		case EMIE_LMOUSE_LEFT_UP:
			MouseState.leftButtonDown = false;
			break;

		case EMIE_MOUSE_MOVED:
			MouseState.position.X = event.MouseInput.X;
			MouseState.position.Y = event.MouseInput.Y;
			break;

		default:
			break;
		}
	}
	else if (event.EventType == EET_KEY_INPUT_EVENT)
	{
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}

	return false;
}
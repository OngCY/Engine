#include "Input.h"

bool InputEventReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
		case EMIE_LMOUSE_LEFT_UP:
			break;
		case EMIE_LMOUSE_DOUBLE_CLICK:
			break;
		case EMIE_MOUSE_MOVED:
			break;
		}
	}
	else if (event.EventType == EET_KEY_INPUT_EVENT)
	{
		//KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}
}
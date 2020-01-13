#include "UI.h"

bool CustomGUIInventory::VOnEvent(const irr::SEvent& event)
{
	if (event.EventType == irr::EET_GUI_EVENT)
	{
		irr::s32 id = event.GUIEvent.Caller->getID();

		switch (event.GUIEvent.EventType)
		{
		case irr::gui::EGET_BUTTON_CLICKED:
			switch (id)
			{
			case GUI_ID_BUTTON_COMBINE:
				//Context.device->closeDevice();
				break;
			}
		}
	}

	return false;
}
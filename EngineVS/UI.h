#pragma once
#include <irrlicht.h>

enum
{
	GUI_ID_BUTTON_COMBINE = 101,
	GUI_ID_BUTTON_APPLY,
	GUI_ID_BUTTON_DISCARD,
	GUI_ID_LIST_ITEMS
};

class CustomGUIInventory : irr::gui::IGUIElement
{
public:
	CustomGUIInventory();
	~CustomGUIInventory();

	virtual bool VOnEvent(const irr::SEvent& event);

private:
	irr::gui::IGUIButton* m_buttonCombine;
	irr::gui::IGUIButton* m_buttonApply;
	irr::gui::IGUIButton* m_buttonDiscard;
	irr::gui::IGUIListBox* m_listItems;
};



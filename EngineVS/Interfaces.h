#pragma once
#include <irrlicht.h>

class IKeyboardHandler
{
	virtual bool VOnKeyPressed(irr::EKEY_CODE kcode) = 0;
};

class IMouseHandler
{
	virtual bool VOnLButtonClick(irr::s32 x, irr::s32 y) = 0;
	virtual bool VOnLButtonDClick(irr::s32 x, irr::s32 y) = 0;
	virtual bool VOnMouseMove(irr::s32 x, irr::s32 y) = 0;
};
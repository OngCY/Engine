#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

class RenderManager
{
public:
	RenderManager() {}
	virtual ~RenderManager() {}

	virtual void VInitRenderer();
	virtual bool VRunRenderer();
	virtual bool VStartScene();
	virtual void VDrawSceneNodes();
	virtual void VDrawUIElements();
	virtual bool VRenderToScreen();
	virtual void VCloseRenderer();

private:
	IrrlichtDevice* device;
	IVideoDriver* videoDriver;
	ISceneManager* sceneMgr;
	IGUIEnvironment* guiEnv;
};

class NullRenderManager : public RenderManager
{
public:
	NullRenderManager() {}
	virtual ~NullRenderManager() {}

	virtual void VInitRenderer() {}
	virtual bool VRunRenderer() { return true; }
	virtual bool VStartScene() { return true; }
	virtual void VDrawSceneNodes() {}
	virtual void VDrawUIElements() {}
	virtual bool VRenderToScreen() { return true; }
	virtual void VCloseRenderer() {}
};
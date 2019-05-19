#include "RenderManager.h"

void RenderManager::VInitRenderer()
{
	device = createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);
	videoDriver = device->getVideoDriver();
	sceneMgr = device->getSceneManager();
	guiEnv = device->getGUIEnvironment();
}

bool RenderManager::VRunRenderer()
{
	return device->run();
}

bool RenderManager::VStartScene()
{
	return videoDriver->beginScene(true, true, SColor(255, 100, 101, 140));
}

void RenderManager::VDrawSceneNodes()
{
	sceneMgr->drawAll();
}

void RenderManager::VDrawUIElements()
{
	guiEnv->drawAll();
}

bool RenderManager::VRenderToScreen()
{
	return videoDriver->endScene();
}

void RenderManager::VCloseRenderer()
{
	device->closeDevice();
	device->drop();
}
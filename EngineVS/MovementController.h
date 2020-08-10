#include <windows.h>
#include <irrlicht.h>
#include <memory>
#include <iostream>
#include "ServiceLocator.h"

class MovementController : public irr::IEventReceiver
{
public:
	MovementController() {}
	MovementController(std::shared_ptr<irr::scene::ISceneNode> node) : m_sceneNode(node) {}
	~MovementController() {}
	void SetObject(std::shared_ptr<irr::scene::ISceneNode> node) { m_sceneNode = node; }
	bool OnEvent(const irr::SEvent& event);

protected:
	void ProcessKeyDown(irr::EKEY_CODE key);
	void ProcessKeyUp(irr::EKEY_CODE key);

	std::shared_ptr<irr::scene::ISceneNode> m_sceneNode;
	bool m_keyDown[irr::KEY_KEY_CODES_COUNT];
};
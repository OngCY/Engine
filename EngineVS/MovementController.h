#include <windows.h>
#include <irrlicht.h>
#include <memory>

class MovementController : public irr::IEventReceiver
{
public:
	MovementController(std::shared_ptr<irr::scene::ISceneNode> node) : m_sceneNode(node) {}
	virtual ~MovementController() {}
	void SetObject(std::shared_ptr<irr::scene::ISceneNode> node) { m_sceneNode = node; }
	virtual bool VOnEvent(const irr::SEvent& event);
	virtual void VOnUpdate(const int deltaMilliseconds);

protected:
	float m_targetYaw;
	float m_targetPitch;
	float m_yaw;
	float m_pitch;
	float m_pitchOnDown;
	float m_yawOnDown;
	std::shared_ptr<irr::scene::ISceneNode> m_sceneNode;

	struct SMouseState
	{
		irr::core::position2di lastPosition;
		bool leftButtonDown;
		SMouseState() :leftButtonDown(false) {}
	};
	SMouseState m_mouseState;
	bool m_keyDown[irr::KEY_KEY_CODES_COUNT];
};
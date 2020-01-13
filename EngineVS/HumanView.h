#include <windows.h>
#include <irrlicht.h>
#include <memory>

using namespace irr;
typedef unsigned int GameViewId;

class HumanView : public IEventReceiver
{
public:
	HumanView() {};
	virtual ~HumanView() {}
	virtual bool VOnEvent(const SEvent& event);
	virtual void VOnUpdate(const int deltaMilliseconds);

	virtual void VSetId(GameViewId viewId) { m_viewId = viewId; }
	virtual GameViewId VGetId() const { return m_viewId; }

	// Virtual methods to control the layering of interface elements
	//virtual void VPushElement(shared_ptr<IScreenElement> pElement);
	//virtual void VRemoveElement(shared_ptr<IScreenElement> pElement);
	//virtual void VOnRender(double fTime, float fElapsedTime);

protected:
	DWORD m_currTick; //requires windows.h 
	DWORD m_lastDraw;
	bool m_runFullSpeed;
	GameViewId m_viewId;

	//ScreenElementList m_ScreenElements;

	// Interface sensitive objects
	//std::shared_ptr<IMouseHandler> m_MouseHandler;
	//int m_PointerRadius;
	//std::shared_ptr<IKeyboardHandler> m_KeyboardHandler;

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);

	struct SMouseState
	{
		core::position2di Position;
		bool leftButtonDown;
		SMouseState() :leftButtonDown(false) {}
	};
	SMouseState m_mouseState;
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
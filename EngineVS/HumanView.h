#include <irrlicht.h>
#include <memory>
#include "Interfaces.h"

using namespace irr;

class HumanView : public IEventReceiver
{
public:
	HumanView() {};
	virtual ~HumanView();
	virtual bool VOnEvent(const SEvent& event);
	virtual void VOnUpdate(const int deltaMilliseconds);

	// Virtual methods to control the layering of interface elements
	//virtual void VPushElement(shared_ptr<IScreenElement> pElement);
	//virtual void VRemoveElement(shared_ptr<IScreenElement> pElement);

protected:
	unsigned long m_currTick;		
	unsigned long m_lastDraw;
	bool m_runFullSpeed;

	//ScreenElementList m_ScreenElements;

	// Interface sensitive objects
	std::shared_ptr<IMouseHandler> m_MouseHandler;
	int m_PointerRadius;
	std::shared_ptr<IKeyboardHandler> m_KeyboardHandler;

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);
};
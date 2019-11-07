#include <irrlicht.h>

using namespace irr;

class InputEventReceiver : public IEventReceiver
{
public:
	InputEventReceiver() {};
	virtual bool OnEvent(const SEvent& event);
};
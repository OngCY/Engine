#include <irrlicht.h>

using namespace irr;

class InputEventReceiver : public IEventReceiver
{
public:
	InputEventReceiver();
	virtual bool OnEvent(const SEvent& event);
	
private:
	struct SMouseState
	{
		core::position2di position;
		bool leftButtonDown;

		SMouseState() : leftButtonDown(false) {}
	} MouseState;

	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
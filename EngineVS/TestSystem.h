#pragma once
#include "ThirdParty\FastDelegate.h"
#include "ThirdParty\FastDelegateBind.h"
#include "ServiceLocator.h"

class TestEventSystem
{
public:
	TestEventSystem();
	bool Init(void);
	void TestEventDelegate(IEventPtr pEvent);
	
};
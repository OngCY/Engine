#pragma once
#include "ThirdParty\FastDelegate.h"
#include "ThirdParty\FastDelegateBind.h"
#include "ServiceLocator.h"
#include <iostream>

class TestEventSystem
{
public:
	TestEventSystem();
	~TestEventSystem();
	void Init(void);
	void DestroyActorDelegate(IEventPtr pEvent);
	
};
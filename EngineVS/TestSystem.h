#pragma once
#include "ThirdParty\FastDelegate.h"
#include "ThirdParty\FastDelegateBind.h"
#include "ServiceLocator.h"
#include "Actors.h"
#include <iostream>

class TestSystem
{
public:
	TestSystem() {}
	~TestSystem();
	void Init(void);
	void DestroyActorDelegate(IEventPtr pEvent);


	
};
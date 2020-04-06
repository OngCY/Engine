#pragma once
#include "ThirdParty\FastDelegate.h"
#include "ThirdParty\FastDelegateBind.h"
#include "ServiceLocator.h"
#include "Actors.h"
#include <iostream>

class GameLogic
{
public:
	GameLogic() {}
	~GameLogic();
	void Init(void);

	//event delegates
	void DestroyActorDelegate(IEventPtr pEvent); //event callback function

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);
};
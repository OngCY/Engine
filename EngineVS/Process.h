#pragma once
#include <memory>
#include <iostream>

class Process;
typedef std::shared_ptr<Process> StrongProcessPtr;
typedef std::weak_ptr<Process> WeakProcessPtr;

class Process
{
	//friend class ProcessManager;

public:
	enum ProcessState
	{
		UNINITIALISED = 0,	//created but not running yet
		REMOVED,			//removed from the process list but not destroyed e.g. assign an already running process to another process
		RUNNING,			//initialised and running
		PAUSED,				//initialised and paused
		SUCCEEDED,			//completed successfully
		FAILED,				//did not complete successfully
		ABORTED				//may not have started
	};

	Process(void);
	virtual ~Process();

	//methods to end the process
	inline void Succeed(void);
	inline void Fail(void);
	inline void Pause(void);
	inline void Unpause(void);

protected:
	virtual void VOnInit(void) { m_processState = RUNNING; }
	virtual void VOnUpdate(unsigned long deltaMS) = 0;
	virtual void VOnSuccess(void) {}
	virtual void VOnFail(void) {}
	virtual void VOnAbort(void) {}

private:
	ProcessState m_processState;
	StrongProcessPtr m_pChild;	//child process, if any
};
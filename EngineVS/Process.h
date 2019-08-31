#pragma once
#include <memory>
#include <iostream>
#include <list>

class Process;
class ProcessManager;
typedef std::shared_ptr<Process> StrongProcessPtr;
typedef std::weak_ptr<Process> WeakProcessPtr;
typedef std::list<StrongProcessPtr> ProcessList;

//////////////////////PROCESSES/////////////////////
class Process
{
	friend class ProcessManager;

public:
	enum ProcessState
	{
		UNINITIALISED = 0,	//created but not running yet
		REMOVED,			//removed from the process list but not destroyed e.g. assign an already running process to another process
		RUNNING,			//initialised and running
		PAUSED,				//initialised and paused
		FINISHED,			//completed successfully
		FAILED,				//did not complete successfully
		ABORTED				//may not have started
	};

	Process() {}
	virtual ~Process() {}

	//access methods
	ProcessState GetState() const { return m_processState; }
	bool IsAlive() const;
	bool IsDead() const;
	bool IsRemoved() const;
	bool IsPaused() const;

	//child process methods
	inline void AttachChildProcess(StrongProcessPtr pChild);
	inline void RemoveChildProcess();
	StrongProcessPtr GetChildProcess() { return m_pChild; }

protected:
	//methods to initialise, end, pause or continue the process
	virtual void VFinish() {}
	virtual void VFail() {}
	virtual void VPause() {}
	virtual void VContinue() {}

	virtual void VOnInit() { m_processState = RUNNING; }
	virtual void VOnUpdate(unsigned long deltaMS) = 0; //child processes must implement this
	virtual void VOnFinish() {}
	virtual void VOnFail() {}
	virtual void VOnAbort() {}

	ProcessState m_processState;

private:
	void SetState(ProcessState state) { m_processState = state; }

	StrongProcessPtr m_pChild;	//child process, if any
};

class DelayProcess : public Process
{
public:
	DelayProcess(unsigned long delay);
	virtual void VFinish();

private:
	unsigned long m_delay;
	unsigned long m_timeDelayedSoFar;

protected:
	virtual void VOnUpdate(unsigned long deltaMs);
};

//////////////////////PROCESS MANAGER/////////////////////
class ProcessManager
{
public:
	~ProcessManager() {}

	//processes
	WeakProcessPtr AttachProcess(StrongProcessPtr pProcess);
	unsigned int UpdateAllProcesses(unsigned long deltaMS);
	void AbortAllProcesses();

	size_t GetNumberOfProcesses() const { return m_processList.size(); }

private:
	void ClearAllProcesses(); //to be called by the destructor only

	ProcessList m_processList;
};

class NullProcessManager : public ProcessManager
{
public:
	NullProcessManager() {}
	~NullProcessManager() {}

	WeakProcessPtr AttachProcess(StrongProcessPtr pProcess) { return WeakProcessPtr(); } //weak_ptr does not have constructors that take nullptr_t or raw pointers
	unsigned int UpdateAllProcessess(unsigned long deltaMS) { return 0; }
	void AbortAllProcesses() {}
};
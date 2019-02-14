#pragma once
#include <memory>
#include <iostream>
#include <list>

class Process;
class ProcessManager;
typedef std::shared_ptr<Process> StrongProcessPtr;
typedef std::weak_ptr<Process> WeakProcessPtr;
typedef std::list<StrongProcessPtr> ProcessList;

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

	//methods to end the process
	inline void Finish();
	inline void Fail();
	
	inline void Pause();
	inline void Unpause();

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
	virtual void VOnInit() { m_processState = RUNNING; }
	virtual void VOnUpdate(unsigned long deltaMS) = 0;
	virtual void VOnFinish() {}
	virtual void VOnFail() {}
	virtual void VOnAbort() {}

private:
	void SetState(ProcessState state) { m_processState = state; }

	ProcessState m_processState;
	StrongProcessPtr m_pChild;	//child process, if any
};

class ProcessManager
{
public:
	~ProcessManager() {}

	//processes
	WeakProcessPtr AttachProcess(StrongProcessPtr pProcess);
	unsigned int UpdateAllProcesses(unsigned long deltaMS);
	void AbortAllProcesses(bool immediate);

	size_t GetNumberOfProcesses() const { return m_processList.size(); }

private:
	void ClearAllProcesses(); //to be called by the destructor only

	ProcessList m_processList;
};
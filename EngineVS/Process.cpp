#include "Process.h"

////////////////////////////PROCESS//////////////////////////////////////
bool Process::IsAlive() const
{
	if (m_processState == RUNNING || m_processState == PAUSED)
		return true;
	else
		return false;
}

bool Process::IsDead() const
{
	if (m_processState == FINISHED || m_processState == FAILED || m_processState == ABORTED)
		return true;
	else
		return false;
}

bool Process::IsPaused() const
{
	if (m_processState == PAUSED)
		return true;
	else
		return false;
}

bool Process::IsRemoved() const
{
	if (m_processState == REMOVED)
		return true;
	else
		return false;
}

void Process::AttachChildProcess(StrongProcessPtr pChild)
{
	m_pChild = pChild;
}

void Process::RemoveChildProcess()
{
	m_pChild.reset(); //decrement the reference count of the shared pointer by 1
}

////////////////////////////DELAY PROCESS//////////////////////////////////////
DelayProcess::DelayProcess(unsigned long delay)
{
	Process::VOnInit(); //call a parent protected method from a derived class method
	m_delay = delay;
	m_timeDelayedSoFar = 0;
}

void DelayProcess::VFinish()
{
	m_processState = Process::FINISHED;
	m_timeDelayedSoFar = 0;
}

void DelayProcess::VOnUpdate(unsigned long deltaMs)
{
	m_timeDelayedSoFar += deltaMs;

	if (m_timeDelayedSoFar >= m_delay)
	{
		VFinish();
	}
}

////////////////////////////PROCESS MANAGER//////////////////////////////////////
WeakProcessPtr ProcessManager::AttachProcess(StrongProcessPtr pProcess)
{
	m_processList.push_back(pProcess);
	WeakProcessPtr wpProcess(pProcess);

	return wpProcess;
}

unsigned int ProcessManager::UpdateAllProcesses(unsigned long deltaMs)
{
	unsigned short int finishCount = 0;
	unsigned short int failCount = 0;

	for (ProcessList::iterator it = m_processList.begin(); it != m_processList.end();)
	{
		StrongProcessPtr pCurrentProccess = (*it);
		ProcessList::iterator currentIt = it;
		++it;

		switch (pCurrentProccess->GetState())
		{
			case Process::UNINITIALISED:
			{
				pCurrentProccess->VOnInit();
				break;
			}
			case Process::RUNNING:
			{
				pCurrentProccess->VOnUpdate(deltaMs);
				break;
			}
			case Process::FINISHED:
			{
				pCurrentProccess->VOnFinish();
				StrongProcessPtr pChild = pCurrentProccess->GetChildProcess();
				
				if (pChild)
				{
					pCurrentProccess->RemoveChildProcess();
					AttachProcess(pChild);
				}
				else
					++finishCount;

				m_processList.erase(currentIt);

				break;
			}
			case Process::FAILED:
			{
				pCurrentProccess->VOnFail();
				++failCount;
				break;
			}
			case Process::ABORTED:
			{
				pCurrentProccess->VOnAbort();
				++failCount;
				break;
			}
		}		
	}

	return ((finishCount << 16) | failCount);
}

void ProcessManager::AbortAllProcesses()
{
	for (ProcessList::iterator it = m_processList.begin(); it != m_processList.end();)
	{
		StrongProcessPtr pCurrentProcess = (*it);
		ProcessList::iterator currentIt = it;
		++it;

		pCurrentProcess->VOnAbort();
		m_processList.erase(currentIt); //deletes the object (in this case, reduces the reference count of the smart pointer. For a raw pointer, the ptr is deleted but not necessarily for the pointee object)
	}
}

void ProcessManager::ClearAllProcesses()
{
	m_processList.clear(); //deletes all objects in the list
}
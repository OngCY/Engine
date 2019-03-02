#include "Process.h"

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
	m_pChild.reset();
}

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
				pCurrentProccess->RemoveChildProcess();

				if (pChild)
					AttachProcess(pChild);
				else
					++finishCount;

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

		m_processList.erase(currentIt);		
	}

	return ((finishCount << 16) | failCount);
}

DelayProcess::DelayProcess(unsigned long delay)
{
	m_delay = delay;
	m_timeDelayedSoFar = 0;
}

void DelayProcess::OnUpdate(unsigned long deltaMs)
{
	m_timeDelayedSoFar += deltaMs;

	if (m_timeDelayedSoFar >= m_delay)
	{
		VFinish();
	}
}
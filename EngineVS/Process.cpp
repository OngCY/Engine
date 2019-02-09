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
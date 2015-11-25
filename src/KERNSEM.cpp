#include "KERNSEM.h"

KernelSem::KernelSem(int initialValue)
{
	id = uniqueID++;
	value = initialValue;
	blockedThreads = new BlockedPCB();
}

KernelSem::~KernelSem()
{
	delete blockedThreads;
}

void KernelSem::wait()
{
	if (--value < 0)
	{
		block();
		numberOfThreadsInScheduler--;
		runningThread = Scheduler::get();
		if (runningThread == NULL) runningThread = idleThread, numberOfThreadsInScheduler++;
	}
}

void KernelSem::signal()
{
	if (value++ < 0)
	{
		numberOfThreadsInScheduler++;
		Scheduler::put(unblock());
	}
}

int KernelSem::val() const
{
	return value;
}

void KernelSem::block()
{
	* blockedThreads += runningThread;
}

PCB * KernelSem::unblock()
{
	return blockedThreads -> pop_first();
}

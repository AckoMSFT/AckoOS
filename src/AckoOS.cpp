#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include "KERNEL.h"
#include "PCB.h"
#include "PARAMS.h"
#include "THREAD.h"
#include "VECTOR.h"
#include "SCHEDULE.h"
#include "IDLE.h"
#include "SEMAPHOR.h"
#include "KERNSEM.h"
#include "BOUNDED.h"
#include "USER.h"
#include "EVENT.h"
#include "KernelEv.h"

int PCB::uniqueID = 0;
int KernelSem::uniqueID = 0;
int KernelEv::uniqueID = 0;

int numberOfThreadsInScheduler = 0;
bool dispatchRequest = false;

PCB * runningThread = new PCB(1024L, 1U);
Kernel * kernelThread = new Kernel(1024L, 0U);
Idle * idleThread = new Idle(1024L, 1U);

Vector<PCB> * threads = new Vector<PCB>(42);
Vector<KernelSem> * semaphores = new Vector<KernelSem>(42);
Vector<KernelEv> * events = new Vector<KernelEv>(256);
SleepyPCB * sleepyThreads = new SleepyPCB();

extern void tick();
extern int userMain(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	asm cli
	cout << "Hello, my name is AckoOS!" << endl;
	asm sti

	saveInterruptRoutinePointers();
	initalizeInterruptRoutinePointers();

	int returnValue = userMain(argc, argv);

	restoreInterruptRoutinePointers();

	asm cli
	cout << "Goodbye, dear user! :(" << endl;
	asm sti

	delete events;
	delete semaphores;
	delete threads;

	delete sleepyThreads;
	delete idleThread;
	delete kernelThread;
	delete runningThread;

	return returnValue;
}




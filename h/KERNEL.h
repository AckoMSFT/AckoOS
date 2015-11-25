#ifndef KERNEL_H_
#define KERNEL_H_

#include <iostream.h>
#include <dos.h>
#include "HELPER.h"
#include "PCB.h"
#include "PARAMS.h"
#include "VECTOR.h"
#include "THREAD.h"
#include "SCHEDULE.h"
#include "IDLE.h"
#include "SEMAPHOR.h"
#include "KERNSEM.h"
#include "KERNELEV.h"

void callSystemCall(SystemCallParameters * systemCallParameters);

void interrupt timer(...);
void interrupt enterSystemCall(...);
void systemCall(...);
void interrupt exitSystemCall(...);

extern int numberOfThreadsInScheduler;
extern bool dispatchRequest;
extern void tick();
extern Idle * idleThread;
extern Vector<PCB> * threads;
extern Vector<KernelSem> * semaphores;
extern Vector<KernelEv> * events;
extern SleepyPCB * sleepyThreads;

void saveInterruptRoutinePointers();
void initalizeInterruptRoutinePointers();
void restoreInterruptRoutinePointers();

static interruptRoutinePointer oldTimerRoutine;
static unsigned currentSS, currentSP, currentBP;

class Kernel : public PCB
{
public:
	Kernel(StackSize __stackSize, Time __timeSlice) : PCB(__stackSize, __timeSlice)
	{
	}
	void prepareStack()
	{
		stack[stackSize - 1] = 0x200;
		stack[stackSize - 2] = FP_SEG(systemCall);
		stack[stackSize - 3] = FP_OFF(systemCall);

		ss = FP_SEG(stack + stackSize - 12);
		sp = FP_OFF(stack + stackSize - 12);
		bp = FP_OFF(stack + stackSize - 12);
	}
};

extern PCB * runningThread;
extern Kernel * kernelThread;

#endif /* KERNEL_H_ */

#include "PCB.h"

PCB::PCB(StackSize __stackSize, Time __timeSlice)
{
	if (stackSize > MAX_STACK_SIZE) stackSize = MAX_STACK_SIZE;
	stackSize = __stackSize / sizeof(unsigned int);
	stack = new unsigned int[stackSize];

	timeSlice = __timeSlice ? __timeSlice : -1;
	remainingTime = timeSlice;

	if (__timeSlice == 0) infiniteRuntime = true;
	else infiniteRuntime = false;

	id = uniqueID++;
	finished = false;
	blockedThreads = new PCBList();
}

PCB::PCB(StackSize __stackSize, Time __timeSlice, Thread * wrapperThread)
{
	if (stackSize > MAX_STACK_SIZE) stackSize = MAX_STACK_SIZE;
	stackSize = __stackSize / sizeof(unsigned int);
	stack = new unsigned int[stackSize];

	timeSlice = __timeSlice ? __timeSlice : -1;
	remainingTime = timeSlice;

	if (__timeSlice == 0) infiniteRuntime = true;
	else infiniteRuntime = false;

	id = uniqueID++;

	stack[stackSize - 1] = FP_SEG(wrapperThread);
	stack[stackSize - 2] = FP_OFF(wrapperThread);
	stack[stackSize - 5] = 0x200;
	stack[stackSize - 6] = FP_SEG(Thread::wrapper);
	stack[stackSize - 7] = FP_OFF(Thread::wrapper);

	ss = FP_SEG(stack + stackSize - 16);
	sp = FP_OFF(stack + stackSize - 16);
	bp = FP_OFF(stack + stackSize - 16);

	wrapperThread -> myPCB = id;
	finished = false;
	blockedThreads = new PCBList();
}

PCB::~PCB()
{
	delete blockedThreads;
	delete[] stack;
}

class Idle;
extern Idle * idleThread;
extern SleepyPCB * sleepyThreads;

void PCB::sleep(Time timeToSleep)
{
	if(timeToSleep > 0)
	{
		sleepyThreads->add(runningThread, timeToSleep);
		numberOfThreadsInScheduler--;
		runningThread = Scheduler::get();
		if(runningThread == NULL) runningThread = (PCB*)idleThread, numberOfThreadsInScheduler++;
	}
}

#include "THREAD.h"

Thread::Thread(StackSize stackSize, Time timeSlice)
{
	SystemCallParameters systemCallParameters(6);
	systemCallParameters.add(THREAD_CREATE).add(FP_SEG(this)).add(FP_OFF(this)).add((stackSize & 0xffff0000UL) >> 16).add(stackSize & 0x0000ffffUL).add(timeSlice);
	callSystemCall(&systemCallParameters);
}

Thread::~Thread()
{
	waitToComplete();
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(THREAD_DISPOSE).add(myPCB);
	callSystemCall(&systemCallParameters);
}

void Thread::start()
{
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(THREAD_START).add(myPCB);
	callSystemCall(&systemCallParameters);
}

extern bool dispatchRequest;
extern bool globalLockedFlag;
extern PCB * runningThread;

void dispatch()
{
	lock();
	dispatchRequest = true;
	callTimer();
	unlock();
}

void Thread::wrapper(Thread * userThread)
{
	userThread -> run();
	lock();
	runningThread -> blockedThreads -> emptyPCBList();
	runningThread -> finished = true;
	dispatch();
}

void Thread::waitToComplete()
{
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(THREAD_WAIT_TO_COMPLETE).add(myPCB);
	callSystemCall(&systemCallParameters);
}

void Thread::sleep(Time timeToSleep)
{
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(THREAD_SLEEP).add(timeToSleep);
	callSystemCall(&systemCallParameters);
}

#ifndef HELPER_H_
#define HELPER_H_

#include <dos.h>
#include "PARAMS.h"

extern void callSystemCall(SystemCallParameters * systemCallParameters);

#define bool int

#define true 1
#define false 0

typedef void interrupt (*interruptRoutinePointer) (...);

typedef unsigned long StackSize;
typedef unsigned int Time;
typedef int ID;

static bool globalLockedFlag = false;
#define lock() globalLockedFlag = true
#define unlock() globalLockedFlag = false

#define callTimer() asm int 08h
#define callEnterSystemCall() asm int 60h
#define callExitSystemCall() asm int 61h
#define callOldTimer() asm int 63h

#define MAX_STACK_SIZE 65536L

enum OPERATION_CODE
{
	THREAD_CREATE,
	THREAD_DISPOSE,
	THREAD_START,
	THREAD_WAIT_TO_COMPLETE,
	THREAD_SLEEP,
	SEMAPHORE_CREATE,
	SEMAPHORE_DISPOSE,
	SEMAPHORE_WAIT,
	SEMAPHORE_SIGNAL,
	SEMAPHORE_VAL,
	EVENT_CREATE,
	EVENT_DISPOSE,
	EVENT_WAIT,
	EVENT_SIGNAL
};

#endif /* HELPER_H_ */

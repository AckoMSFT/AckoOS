#include "KERNEL.h"

void interrupt timer(...)
{
	if (dispatchRequest) unlock();
	else
	{
		runningThread -> remainingTime--;
		sleepyThreads->wakeUp();
	}

	if ((dispatchRequest == true) || (runningThread -> remainingTime == 0 && runningThread -> infiniteRuntime == false))
	{
		if (!globalLockedFlag)
		{
			asm {
				mov currentSS, ss;
				mov currentSP, sp;
				mov currentBP, bp;
			}

			runningThread -> ss = currentSS;
			runningThread -> sp = currentSP;
			runningThread -> bp = currentBP;

			if (runningThread -> finished == false && runningThread != idleThread)
			{
				numberOfThreadsInScheduler++;
				Scheduler::put(runningThread);
			}

			numberOfThreadsInScheduler--;
			runningThread = Scheduler::get();
			if (runningThread == NULL) runningThread = idleThread, numberOfThreadsInScheduler++;

			currentSS = runningThread -> ss;
			currentSP = runningThread -> sp;
			currentBP = runningThread -> bp;

			runningThread -> remainingTime = runningThread -> timeSlice;

			asm {
				mov ss, currentSS;
				mov sp, currentSP;
				mov bp, currentBP;
			}
		}
	}

	if (dispatchRequest == false)
	{
		tick();
		callOldTimer();
	}

	dispatchRequest = false;
}

volatile unsigned int systemCallParametersSegment, systemCallParametersOffset;
SystemCallParameters * systemCallParameters;

void interrupt enterSystemCall(...)
{
	lock();

	asm {
		mov systemCallParametersSegment, ax;
		mov systemCallParametersOffset, bx;
	}

	//asm cli
	//cout << "Hello, my name is enterSystemCall!" << endl;
	//asm sti

	asm {
		mov currentSS, ss;
		mov currentSP, sp;
		mov currentBP, bp;
	}

	runningThread -> ss = currentSS;
	runningThread -> sp = currentSP;
	runningThread -> bp = currentBP;

	kernelThread -> prepareStack();

	currentSS = kernelThread -> ss;
	currentSP = kernelThread -> sp;
	currentBP = kernelThread -> bp;

	asm {
		mov ss, currentSS;
		mov sp, currentSP;
		mov bp, currentBP;
	}
}

void systemCall(...)
{
	//asm cli
	//cout << "Hello, my name is systemCall and this is where the magic happens! :)" << endl;
	//asm sti

	systemCallParameters = (SystemCallParameters *) MK_FP(systemCallParametersSegment, systemCallParametersOffset);
	//asm cli
	//cout << * systemCallParameters << endl;
	//asm sti

	OPERATION_CODE operationCode = systemCallParameters -> parameters[0];
	Thread * thread = NULL;
	Semaphore * semaphore = NULL;
	PCB * pcb = NULL;
	switch (operationCode)
	{
	case THREAD_CREATE:
		//asm cli
//		cout << "THREAD_CREATE" << endl;
		//asm sti
		StackSize stackSize = systemCallParameters -> parameters[3];
		stackSize <<= 16;
		stackSize += systemCallParameters -> parameters[4];
		Time timeSlice = systemCallParameters -> parameters[5];
		//asm cli
		//cout << "stackSize = " << stackSize << endl;
		//cout << "timeSlice = " << timeSlice << endl;
		//asm sti
		thread = (Thread *) MK_FP(systemCallParameters -> parameters[1], systemCallParameters -> parameters[2]);
		PCB * newThread = new PCB(stackSize, timeSlice, thread);
		threads -> add(newThread);
		break;
	case THREAD_DISPOSE:
		//asm cli
		//cout << "THREAD_DISPOSE" << endl;
		//asm sti
		threads -> remove(systemCallParameters -> parameters[1]);
		break;
	case THREAD_START:
		//asm cli
		//cout << "THREAD_START" << endl;
		//asm sti
		numberOfThreadsInScheduler++;
		//thread = (Thread *) MK_FP(systemCallParameters -> parameters[1], systemCallParameters -> parameters[2]);
		//cout << thread->myPCB << endl;
		Scheduler::put(threads -> find(systemCallParameters -> parameters[1]));//thread -> myPCB));
		break;
	case THREAD_WAIT_TO_COMPLETE:
		//cout << "WAIT_TO_COMPLETE" << endl;
		//thread = (Thread *) MK_FP(systemCallParameters -> parameters[1], systemCallParameters -> parameters[2]);
		if (runningThread -> finished == false)
		{
			pcb = threads -> find(systemCallParameters -> parameters[1]);
			//if(!(( threads -> find(thread -> myPCB) )->finished) )
			if(pcb -> finished == false)
			{
				//* ( ( threads -> find(thread -> myPCB) ) -> blockedThreads ) += runningThread;
				* ( pcb -> blockedThreads ) += runningThread;
				numberOfThreadsInScheduler--;
				runningThread = Scheduler::get();
				if (runningThread == NULL) runningThread = (PCB*)idleThread, numberOfThreadsInScheduler++;
			}
		}
		break;
	case THREAD_SLEEP:
		//cout << "THREAD_SLEEP" << endl;
		PCB::sleep(systemCallParameters -> parameters[1]);
		break;
	case SEMAPHORE_CREATE:
		//cout << "SEMAPHORE_CREATE" << endl;
		KernelSem * newSemaphore = new KernelSem(systemCallParameters -> parameters[3]);
		semaphore = (Semaphore *) MK_FP(systemCallParameters -> parameters[1], systemCallParameters -> parameters[2]);
		semaphores -> add(newSemaphore);
		systemCallParameters -> add(newSemaphore -> id);
		break;
	case SEMAPHORE_DISPOSE:
		//cout << "SEMAPHORE_DISPOSE" << endl;
		semaphores -> remove(systemCallParameters -> parameters[1]);
		break;
	case SEMAPHORE_WAIT:
		//cout << "SEMAPHORE_WAIT" << endl;
		semaphores -> find(systemCallParameters -> parameters[1]) -> wait();
		break;
	case SEMAPHORE_SIGNAL:
		//cout << "SEMAPHORE_SIGNAL" << endl;
		semaphores -> find(systemCallParameters -> parameters[1]) -> signal();
		break;
	case SEMAPHORE_VAL:
		//cout << "SEMAPHORE_VAL" << endl;
		systemCallParameters -> add(semaphores -> find(systemCallParameters -> parameters[1]) -> val());
		break;
	case EVENT_CREATE:
		//cout << "EVENT_CREATE" << endl;
		pcb = (PCB *) MK_FP(systemCallParameters -> parameters[1], systemCallParameters -> parameters[2]);
		events -> add(new KernelEv(systemCallParameters -> parameters[3], pcb));
		break;
	case EVENT_DISPOSE:
		//cout << "EVENT_DISPOSE" << endl;
		events -> remove(systemCallParameters -> parameters[1]);
		break;
	case EVENT_WAIT:
		//cout << "EVENT_WAIT" << endl;
		events -> find(systemCallParameters -> parameters[1]) -> wait();
		break;
	case EVENT_SIGNAL:
		//cout << "EVENT_SIGNAL" << endl;
		events -> find(systemCallParameters -> parameters[1]) -> signal();
		break;
	}

	callExitSystemCall();
}

void interrupt exitSystemCall(...)
{
	//asm cli
	//cout << "Hello, my name is exitSystemCall!" << endl;
	//asm sti

	currentSS = runningThread -> ss;
	currentSP = runningThread -> sp;
	currentBP = runningThread -> bp;
	runningThread -> remainingTime = runningThread -> timeSlice;

	asm {
		mov ss, currentSS;
		mov sp, currentSP;
		mov bp, currentBP;
	}

	unlock();
}

void callSystemCall(SystemCallParameters * systemCallParameters)
{
	unsigned int systemCallParametersSegment = FP_SEG(systemCallParameters), systemCallParametersOffset = FP_OFF(systemCallParameters);
	asm {
		mov ax, systemCallParametersSegment;
		mov bx, systemCallParametersOffset;
	}
	callEnterSystemCall();
}

void saveInterruptRoutinePointers()
{
	oldTimerRoutine = getvect(0x08);
}

void initalizeInterruptRoutinePointers()
{
	setvect(0x08, timer);
	setvect(0x60, enterSystemCall);
	setvect(0x61, exitSystemCall);
	setvect(0x63, oldTimerRoutine);
}

void restoreInterruptRoutinePointers()
{
	setvect(0x08, oldTimerRoutine);
	setvect(0x60, NULL);
	setvect(0x61, NULL);
	setvect(0x63, NULL);
}


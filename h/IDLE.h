#ifndef IDLE_H_
#define IDLE_H_

#include "KERNEL.h"

extern int numberOfThreadsInScheduler;

class Idle : public PCB
{
public:
	Idle(StackSize __stackSize, Time __timeSlice) : PCB(__stackSize, __timeSlice)
	{
		prepareStack();
	}
	void prepareStack()
	{
		stack[stackSize - 1] = 0x200;
		stack[stackSize - 2] = FP_SEG(Idle::runIdle);
		stack[stackSize - 3] = FP_OFF(Idle::runIdle);

		ss = FP_SEG(stack + stackSize - 12);
		sp = FP_OFF(stack + stackSize - 12);
		bp = FP_OFF(stack + stackSize - 12);
	}

	static void runIdle()
	{
		while (1)
		{
			if (numberOfThreadsInScheduler > 0)
			{
				dispatch();
			}
		}
	}
};


#endif /* IDLE_H_ */

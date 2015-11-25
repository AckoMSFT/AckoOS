#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "EVENT.h"
#include "KERNSEM.h"
#include "PCB.h"

class KernelEv
{
public:
	static int uniqueID;
	KernelEv(IVTNo ivtNo, PCB * thread);
	~KernelEv();
	void wait();
	void signal();
private:
	KernelSem * semaphore;
	IVTNo ivtNo;
	PCB * thread;
public:
	ID id;
};

#endif /* KERNELEV_H_ */

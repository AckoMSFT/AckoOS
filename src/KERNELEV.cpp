#include "KERNELEV.h"
#include "EVENT.h"

KernelEv::KernelEv(IVTNo ivtNo, PCB * thread)
{
	id = uniqueID++;
	this -> ivtNo = ivtNo;
	this -> thread = thread;
	this -> semaphore = new KernelSem(0);
	IVTEntry::ivtEntries[ivtNo] -> event = this;
	InterruptRoutinePointer * pointer = (InterruptRoutinePointer *)(ivtNo * 4);
	IVTEntry::ivtEntries[ivtNo]->oldInterruptRoutine = * pointer;
	* pointer = IVTEntry::ivtEntries[ivtNo] -> newInterruptRoutine;
}

KernelEv::~KernelEv()
{
	delete semaphore;
	InterruptRoutinePointer * pointer = (InterruptRoutinePointer *)(ivtNo * 4);
	* pointer = IVTEntry::ivtEntries[ivtNo]->oldInterruptRoutine;
}

void KernelEv::wait()
{
	if(runningThread == thread) semaphore -> wait();
}

void KernelEv::signal()
{
	semaphore -> signal();
}

IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutinePointer newInterruptRoutine)
{
	this -> ivtNo = ivtNo;
	this -> newInterruptRoutine = newInterruptRoutine;
	this -> event = NULL;
	IVTEntry::ivtEntries[ivtNo] = this;
}

IVTEntry::~IVTEntry()
{
	IVTEntry::ivtEntries[ivtNo] = NULL;
	oldInterruptRoutine();
}

void IVTEntry::signal()
{
	IVTEntry::ivtEntries[ivtNo] -> event -> signal();
}

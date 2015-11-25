#ifndef EVENT_H_
#define EVENT_H_

#include "HELPER.h"

typedef unsigned char IVTNo;
class KernelEv;

typedef void interrupt (*InterruptRoutinePointer)();

class Event
{
public:
	Event(IVTNo ivtNo);
	~Event();
	void wait();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	ID id;
};

class IVTEntry
{
public:
	IVTEntry(IVTNo ivtNo, InterruptRoutinePointer newInterruptRoutine);
	~IVTEntry();
	void signal();
	static IVTEntry * ivtEntries[256];
	InterruptRoutinePointer oldInterruptRoutine, newInterruptRoutine;
	IVTNo ivtNo;
	KernelEv * event;
};


#define PREPAREENTRY(ivtNo,callOldInterruptRoutine)\
extern IVTEntry ivtEntry##ivtNo;\
\
void interrupt eventInterrupt##ivtNo(){\
	if (callOldInterruptRoutine) ivtEntry##ivtNo.oldInterruptRoutine();\
	ivtEntry##ivtNo.signal();\
}\
\
IVTEntry ivtEntry##ivtNo(ivtNo,eventInterrupt##ivtNo)

#endif /* EVENT_H_ */

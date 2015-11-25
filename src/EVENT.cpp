#include "EVENT.h"
#include "PCB.h"

IVTEntry* IVTEntry::ivtEntries[256];
extern PCB * runningThread;

Event::Event(IVTNo ivtNo)
{
	SystemCallParameters systemCallParameters(5);
	systemCallParameters.add(EVENT_CREATE).add(FP_SEG(runningThread)).add(FP_OFF(runningThread)).add(ivtNo);
	callSystemCall(&systemCallParameters);
	id = systemCallParameters.parameters[4];
}

Event::~Event()
{
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(EVENT_DISPOSE).add(id);
	callSystemCall(&systemCallParameters);
}

void Event::wait()
{
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(EVENT_WAIT).add(id);
	callSystemCall(&systemCallParameters);
}

void Event::signal()
{
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(EVENT_SIGNAL).add(id);
	callSystemCall(&systemCallParameters);
}

#include "SEMAPHOR.h"

Semaphore::Semaphore(int init)
{
	SystemCallParameters systemCallParameters(5);
	systemCallParameters.add(SEMAPHORE_CREATE).add(FP_SEG(this)).add(FP_OFF(this)).add(init);
	callSystemCall(&systemCallParameters);
	id = systemCallParameters.parameters[4];
}

Semaphore::~Semaphore()
{
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(SEMAPHORE_DISPOSE).add(id);
	callSystemCall(&systemCallParameters);
}

void Semaphore::wait()
{
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(SEMAPHORE_WAIT).add(id);
	callSystemCall(&systemCallParameters);
}

void Semaphore::signal()
{
	SystemCallParameters systemCallParameters(2);
	systemCallParameters.add(SEMAPHORE_SIGNAL).add(id);
	callSystemCall(&systemCallParameters);
}

int Semaphore::val() const
{
	SystemCallParameters systemCallParameters(3);
	systemCallParameters.add(SEMAPHORE_VAL).add(id);
	callSystemCall(&systemCallParameters);
	return systemCallParameters.parameters[2];
}

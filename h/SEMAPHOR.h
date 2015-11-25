#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

#include "HELPER.h"

typedef int ID;

class KernelSem;

class Semaphore
{
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();
	virtual void wait();
	virtual void signal();
	int val() const;
private:
	ID id;
};

#endif /* SEMAPHOR_H_ */

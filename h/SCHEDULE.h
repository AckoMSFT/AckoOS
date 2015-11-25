#ifndef SCHEDULE_H_
#define SCHEDULE_H_

class PCB;

class Scheduler
{
public:
	static void put(PCB *);
	static PCB* get();
};

#endif /* SCHEDULE_H_ */

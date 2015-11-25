#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "PCB.h"
#include "HELPER.h"
#include "KERNEL.h"
#include "SCHEDULE.h"

class BlockedPCB;

extern int numberOfThreadsInScheduler;
extern PCB * runningThread;

class KernelSem
{
	static int uniqueID;
public:
	KernelSem(int initialValue = 1);
	virtual ~KernelSem();
	virtual void wait();
	virtual void signal();
	int val() const;
	void block();
	PCB * unblock();
	int value;
	BlockedPCB * blockedThreads;
	ID id;
};

class BlockedPCB
{
	class Node
	{
	public:
		PCB * value;
		Node * next;
		Node(PCB * __value, Node * __next = NULL)
		{
			value = __value;
			next = __next;
		}
	};
	Node * first, * last;
public:
	BlockedPCB()
	{
		first = last = NULL;
	}
	virtual ~BlockedPCB()
	{
		while ( first != NULL )
		{
			Node * current = first;
			first = first -> next;
			delete current;
		}
		first = last = NULL;
	}
	BlockedPCB& operator += (PCB * value)
	{
		if ( first == NULL ) first = last = new Node(value);
		else last = last -> next = new Node(value);
		return * this;
	}

	PCB * pop_first()
	{
		PCB * current = first -> value;
		first = first -> next;
		return current;
	}
};

#endif /* KERNSEM_H_ */

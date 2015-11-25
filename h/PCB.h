#ifndef PCB_H_
#define PCB_H_

#include "HELPER.h"
#include "THREAD.h"
#include "SCHEDULE.h"
#include <dos.h>

class PCBList;

class PCB
{
	static int uniqueID;
protected:
	friend class Thread;
public:
	PCB(StackSize __stackSize, Time __timeSlice);
	PCB(StackSize __stackSize, Time __timeSlice, Thread * wrapperThread);
	virtual ~PCB();
	unsigned int ss, sp, bp, * stack;
	StackSize stackSize;
	Time timeSlice, remainingTime;
	bool infiniteRuntime;
	bool finished;
	ID id;
	PCBList * blockedThreads;
	static void sleep(Time timeToSleep);
};

extern int numberOfThreadsInScheduler;
extern PCB * runningThread;

class PCBList
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
	PCBList()
	{
		first = last = NULL;
	}
	virtual ~PCBList()
	{
		while ( first != NULL )
		{
			Node * current = first;
			first = first -> next;
			delete current;
		}
		first = last = NULL;
	}
	PCBList& operator += (PCB * value)
	{
		if ( first == NULL ) first = last = new Node(value);
		else last = last -> next = new Node(value);
		return * this;
	}
	void emptyPCBList()
	{
		while (first != NULL)
		{
			Node * current = first;
			numberOfThreadsInScheduler++;
			Scheduler::put(current -> value);
			first = first -> next;
			delete current;
		}
		first = last = NULL;
	}
};


class SleepyPCB
{
	Time currentTime;
	class Node
	{
	public:
		PCB * value;
		Node * next;
		Time wakeUpTime;
		Node(PCB * __value, Time __wakeUpTime)
		{
			value = __value;
			next = NULL;
			wakeUpTime = __wakeUpTime;
		}
	};
	Node * first, * last;
public:
	SleepyPCB()
	{
		first = last = NULL;
	}
	virtual ~SleepyPCB()
	{
		while ( first != NULL )
		{
			Node * current = first;
			first = first -> next;
			delete current;
		}
		first = last = NULL;
	}
	void add(PCB * value, Time timeToSleep)
	{
		Node * newNode = new Node(value, currentTime + timeToSleep);
		Node * current = first, * previous = NULL;
		while ((current != NULL) && (current -> wakeUpTime < newNode -> wakeUpTime))
		{
			previous = current;
			current = current -> next;
		}
		if (previous != NULL) previous -> next = newNode;
		else first = newNode;
		newNode -> next = current;
	}

	void wakeUp()
	{
		Node * current = first, * temp = NULL;
		if ( current != NULL) currentTime++;
		else currentTime = 0;

		while ((current != NULL) && (current -> wakeUpTime <= currentTime))
		{
			temp = current;
			numberOfThreadsInScheduler++;
			Scheduler::put(current -> value);
			current = current -> next;
			first = current;
			delete temp;
		}
	}
};

#endif /* PCB_H_ */

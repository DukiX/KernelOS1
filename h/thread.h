#ifndef _THREAD_H_
#define _THREAD_H_
#include "makroi.h"

class PCB;
class Red;

typedef unsigned long StackSize;
typedef unsigned int Time;
typedef int ID;

const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2;

class Thread{
public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

	static Red* redNiti;

protected:
	friend class PCB;
	friend class Idle;
	friend class Timer;
	Thread(StackSize stackSize = defaultStackSize,Time timeSlice = defaultTimeSlice);
	virtual void run(){};

private:
	PCB* myPCB;

};

void dispatch();

extern void tick();

#endif /* _THREAD_H_ */

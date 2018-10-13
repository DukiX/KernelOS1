#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"

class Thread;

class Idle:public Thread{
public:
	Idle();
	virtual void run();
	void start();
};



#endif /* IDLE_H_ */

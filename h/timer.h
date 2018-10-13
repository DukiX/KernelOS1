#ifndef TIMER_H_
#define TIMER_H_

#include "idle.h"

class Timer{
public:

	volatile static char lockFlag;

	Timer();
	~Timer();

	static void zameni();
	static void obnovi();

	static void lockF() { lockFlag++; }
	static void unlockF() { lockFlag--; }

	static void interrupt timer(...);
};

#endif /* TIMER_H_ */

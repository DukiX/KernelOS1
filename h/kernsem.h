#ifndef KERNSEM_H_
#define KERNSEM_H_
#include "red.h"
#include "semred.h"

extern SemRed* redSaVrem;

class Red;

class KernelSem{
public:
	KernelSem(int init=1);
	~KernelSem();

	int wait(Time maxTimeToWait);
	void signal();
	void signalAll();
	int block(Time maxTimeToWait);
	void unblock();

	void signalTimer(PCB* p);

protected:
	friend class Semaphore;
private:
	int vr;
	Red* red;
};



#endif /* KERNSEM_H_ */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "red.h"
#include "SCHEDULE.H"

class Thread;
class Red;


class PCB{
private:
	unsigned* stek;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	Time kvant;
	StackSize velSteka;
	volatile int status;

	Thread* mojaNit;
	ID id;
	static ID brID;

	Red* mojRed;

	int semIsteklo;
protected:
	friend class Thread;
	friend class Timer;
	friend class KernelSem;
	friend class SemRed;
public:
	static volatile Time brojac;
	static volatile int zahtevana_promena_konteksta;
	volatile static PCB* running;
	static Idle* idle;
	static Thread* startnaNit;


	PCB(Thread* mojanit, StackSize stvel,Time vreme);
	~PCB();

	static void runOmotac();

	ID dohvID(){
		return id;
	}

	Thread* dohvNit(){
		return mojaNit;
	}

	int dohvStatus(){return status;}
	void postaviStatus(int stat){
		status=stat;
	}

	Time dohvKvant(){
		return kvant;
	}

	void napraviPCB();
};



#endif /* PCB_H_ */

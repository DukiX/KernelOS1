
#ifndef SEMRED_H_
#define SEMRED_H_

#include "pcb.h"
#include "semaphor.h"

class PCB;

class SemElem{
public:
	PCB* p;
	SemElem* sled;
	unsigned long int vreme;
	KernelSem* s;
};

class SemRed{
private:
	SemElem* glava;
protected:
	friend class Timer;
public:
	SemRed():glava(0){};
	~SemRed();

	void stavi(PCB* p,Time maxTime,KernelSem* s);
	void izvadi(PCB* t);
	void izvadiNaIstek(SemElem* semEl);

};


#endif /* SEMRED_H_ */

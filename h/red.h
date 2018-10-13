#ifndef RED_H_
#define RED_H_

#include "pcb.h"
#include "thread.h"

class PCB;

class Elem{
public:
	PCB* p;
	Elem* sled;
	Elem* pret;
};

class Red{
private:
	Elem* prvi,*posl;
	int duz;
public:
	Red():prvi(0),posl(0),duz(0){}
	~Red();

	void stavi(PCB* p);
	PCB* uzmi();
	PCB* uzmiNitPoID(ID id);
	PCB* uzmiNitPoIDsaVadjenjem(ID id);

	int vel()const{return duz;}
};

#endif /* RED_H_ */

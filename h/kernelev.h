#ifndef KERNELEV_H_
#define KERNELEV_H_

class PCB;
class Event;

typedef unsigned char IVTno;

class KernelEv{
public:
	KernelEv(IVTno ivtNo);
	~KernelEv();

	void wait();
	void signal();
private:
	int vredn;
	int blokirana;
	IVTno brUlaza;
	PCB* pcb;
};



#endif /* KERNELEV_H_ */

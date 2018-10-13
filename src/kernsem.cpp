#include "kernsem.h"
#include "makroi.h"
#include "timer.h"

SemRed* redSaVrem = new SemRed();

KernelSem::KernelSem(int init){
	Timer::lockF();
	vr=init;
	red = new Red();
	Timer::unlockF();
}

KernelSem::~KernelSem(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	//Timer::lockF();
	if(red){
		signalAll();
		delete red;
	}
	//Timer::unlockF();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

int KernelSem::wait(Time maxTimeToWait){
	//Timer::lockF();
#ifndef	BCC_BLOCK_IGNORE
	lock
#endif
	int ret=1;
	if(--vr<0)
		ret = block(maxTimeToWait);
	//Timer::unlockF();
#ifndef	BCC_BLOCK_IGNORE
	unlock
#endif
	return ret;
}

int KernelSem::block(Time maxTimeToWait){
	((PCB*)PCB::running)->postaviStatus(BLOKIRANA);
	red->stavi((PCB*)PCB::running);
	if(maxTimeToWait!=0){
		redSaVrem->stavi((PCB*)PCB::running,maxTimeToWait,this);
	}
	//Timer::unlockF();
	dispatch();
	//Timer::lockF();
	if(maxTimeToWait!=0){ // dal je isao u onaj red
		if((PCB::running)->semIsteklo)
			return 0;
		else return 1;
	}
	else{
		return 1;
	}

}
void KernelSem::signal(){
	//Timer::lockF();
#ifndef	BCC_BLOCK_IGNORE
	lock
#endif
	if(vr++<0)
		unblock();
#ifndef	BCC_BLOCK_IGNORE
	unlock
#endif
	//Timer::unlockF();
}

void KernelSem::unblock(){
	PCB* t=red->uzmi();
	redSaVrem->izvadi(t); //ako je u redu izvadi ga i sredi vremena, inace nista
	t->postaviStatus(SPREMNA);
	Scheduler::put(t);
}

void KernelSem::signalAll(){
	while(red->vel()>0){
		PCB* t=red->uzmi();
		redSaVrem->izvadi(t);
		t->postaviStatus(SPREMNA);
		Scheduler::put(t);
	}
}

void KernelSem::signalTimer(PCB* p){
	if(vr++<0)
		red->uzmiNitPoIDsaVadjenjem(p->dohvID());
}

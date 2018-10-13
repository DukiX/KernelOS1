#include "kernelev.h"
#include "makroi.h"
#include "pcb.h"
#include "ivtentry.h"
#include "timer.h"

KernelEv::KernelEv(IVTno ivtNo){
	Timer::lockF();
	IVTEntry::uzmi(ivtNo)->kernelDog = this;
	brUlaza = ivtNo;
	vredn = 0;
	blokirana=0;
	pcb = (PCB*)PCB::running;
	Timer::unlockF();
}

KernelEv::~KernelEv(){
	Timer::lockF();
	IVTEntry::uzmi(brUlaza)->kernelDog = 0;
	pcb = 0;
	Timer::unlockF();
}

void KernelEv::wait(){
	//Timer::lockF();
#ifndef	BCC_BLOCK_IGNORE
	lock
#endif
	if(PCB::running == pcb){
		if(vredn==0){
		vredn = 0;
		blokirana=1;
		pcb->postaviStatus(BLOKIRANA);
		//Timer::unlockF();
		dispatch();
		//Timer::lockF();
		} else{
			vredn=0;
		}
	}
	//Timer::unlockF();
#ifndef	BCC_BLOCK_IGNORE
	unlock
#endif
}

void KernelEv::signal(){
	//Timer::lockF();
#ifndef	BCC_BLOCK_IGNORE
	lock
#endif
	if(blokirana == 1){
		vredn = 0;
		blokirana=0;
		pcb->postaviStatus(SPREMNA);
		Scheduler::put(pcb);
	} else {
		vredn=1;
	}
	//Timer::unlockF();
#ifndef	BCC_BLOCK_IGNORE
	unlock
#endif
}

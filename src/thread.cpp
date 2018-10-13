#include "thread.h"
#include "makroi.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "timer.h"

Red* Thread::redNiti = new Red();

Thread::Thread(StackSize stackSize,Time timeSlice){
	//Timer::lockF();
	myPCB = new PCB(this,stackSize,timeSlice);
	redNiti->stavi(myPCB);
	//Timer::unlockF();
}

Thread::~Thread(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	waitToComplete();
	PCB* t=redNiti->uzmiNitPoIDsaVadjenjem(myPCB->dohvID());
	delete t;
	myPCB = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Thread::start(){
	Timer::lockF();
	if(myPCB->dohvStatus() == NOVA){
		myPCB->postaviStatus(SPREMNA);
		myPCB->napraviPCB();
		Scheduler::put(myPCB);
	}
	Timer::unlockF();
}

void Thread::waitToComplete(){
	Timer::lockF();
	if(myPCB->dohvStatus() == RADI || myPCB->dohvStatus() == GOTOVA ||
			this == PCB::idle || myPCB->dohvStatus() == NOVA || this == PCB::startnaNit){
		Timer::unlockF();
		return;
	}
	((PCB*)PCB::running)->postaviStatus(BLOKIRANA);
	myPCB->mojRed->stavi((PCB*)PCB::running);
	Timer::unlockF();
	dispatch();
}

Thread* Thread::getThreadById(ID id){
	Timer::lockF();
	PCB* t=Thread::redNiti->uzmiNitPoID(id);
	Thread* tp=t->dohvNit();
	Timer::unlockF();
	return tp;
}

void dispatch(){
	PCB::zahtevana_promena_konteksta = 1;
	//Timer::timer();
#ifndef	BCC_BLOCK_IGNORE
	asm int 08h;
#endif
	PCB::zahtevana_promena_konteksta = 0;
}

ID Thread::getId(){
	return myPCB->id;
}

ID Thread::getRunningId(){
	Timer::lockF();
	ID t=PCB::running->id;
	Timer::unlockF();
	return t;
}


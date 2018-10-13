#include "pcb.h"
#include "makroi.h"
#include <dos.h>
#include "makroi.h"
#include "timer.h"

volatile Time PCB::brojac = 0;
volatile int PCB::zahtevana_promena_konteksta = 0;
volatile PCB* PCB::running = 0;
Idle* PCB::idle = 0;
Thread* PCB::startnaNit = 0;
ID PCB::brID = 0;

PCB::PCB(Thread* mojanit,StackSize stvel,Time kv){
	//Timer::lockF();
	mojaNit = mojanit;
	mojRed = new Red();
	id=++brID;
	if(stvel>MAX_VEL){
		velSteka = MAX_VEL/sizeof(unsigned);
	}else velSteka = stvel/sizeof(unsigned);
	kvant = kv;
	status = NOVA;

	semIsteklo = 1;

	bp=0; ss=0; sp=0; stek=0;
	//Timer::unlockF();
}

PCB::~PCB(){
	Timer::lockF();
	if(stek)delete stek;
	if(mojRed)delete mojRed;
	Timer::unlockF();
}

void PCB::runOmotac(){
	running->mojaNit->run();
	//Timer::lockF();
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	running->status = GOTOVA;
	while(running->mojRed->vel() > 0){
		PCB* t = running->mojRed->uzmi();
		t->postaviStatus(SPREMNA);
		Scheduler::put(t);
	}
	//Timer::unlockF();
	dispatch();
}


void PCB::napraviPCB(){
	stek = new unsigned[velSteka];
#ifndef	BCC_BLOCK_IGNORE
	stek[velSteka - 2] = 0x200;

	stek[velSteka - 3] = FP_SEG(&(runOmotac));
	stek[velSteka - 4] = FP_OFF(&(runOmotac));

	stek[velSteka - 13] = FP_OFF(stek + velSteka - 1);

	ss = FP_SEG(stek + velSteka - 13);
	sp = FP_OFF(stek + velSteka - 13);
	bp=sp;
#endif
}


#include "idle.h"
#include "makroi.h"
#include "pcb.h"

Idle::Idle():Thread(){}

void Idle::run(){
	while(1) {}
}

void Idle::start(){
	myPCB->postaviStatus(SPREMNA);
	myPCB->napraviPCB();
}



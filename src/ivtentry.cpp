#include "ivtentry.h"
#include <dos.h>
#include "makroi.h"
#include "timer.h"

IVTEntry* IVTEntry::ivt[256];

IVTEntry::IVTEntry(IVTno br, rutina rut){
	Timer::lockF();
	ivt[br] = this;
	kernelDog = 0;
	brojUlaza = br;
	staraRutina = 0;
#ifndef BCC_BLOCK_IGNORE
	staraRutina = getvect(br);
	setvect(br,rut);
#endif
	Timer::unlockF();
}

IVTEntry::~IVTEntry(){
	Timer::lockF();
#ifndef BCC_BLOCK_IGNORE
	setvect(brojUlaza,staraRutina);
#endif
	Timer::unlockF();
}

IVTEntry* IVTEntry::uzmi(IVTno br){
	return ivt[br];
}

void IVTEntry::pozoviStaru(){
	staraRutina();
}

#include "timer.h"
#include "makroi.h"
#include <dos.h>
#include "thread.h"
#include "kernsem.h"


volatile unsigned stariSEG, stariOFF;
volatile unsigned noviSEG, noviOFF;
volatile unsigned tss,tsp,tbp;

volatile char Timer::lockFlag = 0;

Timer::Timer(){
	Timer::zameni();
	PCB::idle = new Idle();
	PCB::idle->start();
	PCB::startnaNit = new Thread();
	PCB::startnaNit->myPCB->postaviStatus(RADI);
	PCB::running = PCB::startnaNit->myPCB;
}

Timer::~Timer(){
	Timer::obnovi();
	delete PCB::idle;
	delete PCB::startnaNit;
	delete redSaVrem;
}

void Timer::zameni(){
#ifndef	BCC_BLOCK_IGNORE
	noviSEG = FP_SEG(Timer::timer);
	noviOFF = FP_OFF(Timer::timer);
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr stariSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr stariOFF, ax

		mov ax, noviSEG
		mov word ptr es:0022h, ax	 //postavlja
		mov ax, noviOFF
		mov word ptr es:0020h, ax //novu rutinu

		mov ax, stariSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, stariOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
	#endif
}

void Timer::obnovi(){
#ifndef	BCC_BLOCK_IGNORE
	asm{
	        cli
	        push es
	        push ax

	        mov ax,0
	        mov es,ax

	        mov ax,word ptr stariSEG
	        mov word ptr es:0022h, ax
	        mov ax, word ptr stariOFF
	        mov word ptr es:0020h, ax

	        pop ax
	        pop es
	        sti
	    }
#endif
}

void interrupt Timer::timer(...){
	if(PCB::zahtevana_promena_konteksta == 0){
#ifndef	BCC_BLOCK_IGNORE
		asm int 60h;
#endif
		tick();
		if(redSaVrem->glava!=0){
			redSaVrem->glava->vreme--;
			while(redSaVrem->glava!=0 && redSaVrem->glava->vreme==0){
				SemElem* tek = redSaVrem->glava;
				redSaVrem->glava = redSaVrem->glava->sled;
				redSaVrem->izvadiNaIstek(tek);
				delete tek;
			}
		}
	}
	if(!PCB::zahtevana_promena_konteksta && ((PCB*)PCB::running)->dohvKvant()!=0 && lockFlag == 0){
		PCB::brojac++;
	} //PCB::zahtevana_promena_konteksta ||
	if((PCB::zahtevana_promena_konteksta && Timer::lockFlag == 0) || ((Timer::lockFlag == 0) && (((PCB*)PCB::running)->dohvKvant()!=0) && (((PCB*)PCB::running)->dohvKvant() == PCB::brojac))){

#ifndef	BCC_BLOCK_IGNORE
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif

		PCB::running->ss = tss;
		PCB::running->sp = tsp;
		PCB::running->bp = tbp;

		if(((PCB*)PCB::running)->dohvStatus() == RADI && (PCB::running != PCB::idle->myPCB)){
			((PCB*)PCB::running)->postaviStatus(SPREMNA);
			Scheduler::put((PCB*)PCB::running);
		}

		PCB::running = Scheduler::get();

		if(PCB::running == 0){
			PCB::running = PCB::idle->myPCB;
		}

		((PCB*)PCB::running)->postaviStatus(RADI);

		tss = PCB::running->ss;
		tsp = PCB::running->sp;
		tbp = PCB::running->bp;

#ifndef	BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif

		PCB::brojac = 0;
		PCB::zahtevana_promena_konteksta = 0;

	}

}

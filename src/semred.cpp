#include "semred.h"
#include "kernsem.h"
#include "makroi.h"


SemRed::~SemRed(){
	SemElem* tek;
	while(glava){
		tek=glava;
		glava=glava->sled;
		delete tek;
	}
}

void SemRed::stavi(PCB* p,Time maxTime,KernelSem* s){
	p->semIsteklo = 1; //ponovo postavljanje na 1
	if(glava == 0){
		glava = new SemElem();
		glava->p = p;
		glava->sled = 0;
		glava->vreme = maxTime;
		glava->s = s;
	}
	else{
		int brVrem = 0;
		SemElem* tek=glava;
		SemElem* preth = 0;
		while(tek!= 0){
			brVrem+=tek->vreme;
			if(brVrem > maxTime) break;
			preth = tek;
			tek=tek->sled;
		}
		SemElem* novi=new SemElem();
		novi->p = p;
		novi->s = s;
		if(preth == 0){
			novi->sled = glava;
			glava = novi;
			glava->vreme = maxTime;
			SemElem* t = glava->sled; //od drugog
			if(t != 0){
				t->vreme -= maxTime;  //oduzimanje ostalima
			}
		}
		else if(tek == 0){
			preth->sled = novi;
			novi->sled = 0;
			novi->vreme = maxTime - brVrem;
		}
		else{
			novi->sled = tek;
			preth->sled = novi;
			int vr = maxTime - (brVrem - tek->vreme);
			novi->vreme = vr;
			tek->vreme -= vr;
		}
	}
}

void SemRed::izvadi(PCB* t){
	SemElem* tek = glava;
	SemElem* preth = 0;
	while(tek->p != t) {
		preth = tek;
		tek = tek->sled;
		if(tek == 0)break;
	}
	if(tek == 0){
		return;
	}
	tek->p->semIsteklo = 0; // POSTAVI NA 0 SEMISTEKLO
	if(tek == glava){
		if(glava->sled != 0){
			glava = glava->sled;
			glava->vreme += tek->vreme;
			delete tek;
		}
		else {
			delete glava;
			glava = 0;
		}
	}
	else{
		preth->sled = tek->sled;
		if(tek->sled != 0){
			tek->sled->vreme += tek->vreme;
		}
		delete tek;
	}
}

void SemRed::izvadiNaIstek(SemElem* semEl){
	PCB* p = semEl->p;
	semEl->s->signalTimer(p);
	p->semIsteklo = 1;
	p->postaviStatus(SPREMNA);
	Scheduler::put(p);
}

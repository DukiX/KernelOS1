#include "red.h"

Red::~Red(){
	Elem* tek;
	while(prvi){
		tek=prvi;
		prvi=prvi->sled;
		delete tek;
	}
	posl=0;
	duz=0;
}

void Red::stavi(PCB* p){
	Elem* tek = prvi;
	while(tek != 0){
		if(tek->p == p)break;
		tek = tek->sled;
	}
	if(tek != 0) {
		return;
	}

	if(prvi == 0){
		Elem* novi=new Elem();
		prvi = posl =novi;
		prvi->p = p;
		prvi->sled = 0;
		prvi->pret = 0;
		duz++;
	}
	else{
		posl->sled = new Elem();
		Elem* t = posl;
		posl = posl->sled;
		posl->pret = t;
		posl->sled = 0;
		posl->p=p;
		duz++;
	}
}

PCB* Red::uzmi(){
	if(prvi == 0){
		return 0;
	}
	else{
		if(prvi->sled == 0){
			Elem* te = prvi;
			PCB* tp = prvi->p;
			prvi = posl = 0;
			duz=0;
			delete te;
			return tp;
		}
		else{
			Elem* te = prvi;
			prvi = prvi->sled;
			prvi->pret = 0;
			PCB* tp = te->p;
			duz--;
			delete te;
			return tp;
		}
	}
}

PCB* Red::uzmiNitPoID(ID id){
		if(prvi == 0){
			return 0;
		}
		else{
			Elem* tek = prvi;
			while(tek != 0){
				if(tek->p->dohvID() == id) break;
				tek = tek->sled;
			}
			if(tek == 0){
				return 0;
			}
			return tek->p;
		}
}

PCB* Red::uzmiNitPoIDsaVadjenjem(ID id){
	if(prvi == 0){
		return 0;
	}
	else{
		Elem* tek = prvi;
		while(tek != 0){
			if(tek->p->dohvID() == id) break;
			tek = tek->sled;
		}
		if(tek == 0){
			return 0;
		}
		PCB* tp = tek->p;
		if(tek->pret == 0){
			if(tek->sled == 0){
				prvi = posl = 0;
				duz = 0;
				delete tek;
				return tp;
			}
			else{
				prvi = prvi->sled;
				prvi->pret = 0;
				duz--;
				delete tek;
				return tp;
			}
		}
		else if(tek->sled == 0){
			posl = posl->pret;
			posl->sled = 0;
			duz--;
			delete tek;
			return tp;
		}
		else {
			tek->pret->sled=tek->sled;
			tek->sled->pret=tek->pret;
			duz--;
			delete tek;
			return tp;
		}
	}
}

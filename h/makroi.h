#ifndef MAKROI_H_
#define MAKROI_H_

#include "ivtentry.h"
#include "kernelev.h"

#define MAX_VEL 65535
//32767
//16384

#define lock asm pushf; asm cli
#define unlock asm popf

//#define lock {asm{ cli }}
//#define unlock {asm{ sti }}

#define NOVA 0
#define SPREMNA 1
#define BLOKIRANA 2
#define GOTOVA 3
#define RADI 4

#define PREPAREENTRY(brojUl, zoviStaru)\
void interrupt inter##brojUl(...);\
IVTEntry noviUlaz##brojUl(brojUl, inter##brojUl);\
void interrupt inter##brojUl(...){\
	if(noviUlaz##brojUl.kernelDog !=0) noviUlaz##brojUl.kernelDog->signal();\
	if (zoviStaru == 1)\
		noviUlaz##brojUl.pozoviStaru();\
}


#endif /* MAKROI_H_ */

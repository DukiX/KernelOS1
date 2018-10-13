#include "timer.h"
#include <stdio.h>

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	Timer* timer = new Timer();
	int vr = userMain(argc,argv);
	delete timer;
	printf("userMain returned %d",vr);
	return vr;
}

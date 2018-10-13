#include "semaphor.h"
#include "makroi.h"
#include "kernsem.h"
#include "timer.h"

Semaphore::Semaphore(int init){
	//Timer::lockF();
	myImpl = new KernelSem(init);
	//Timer::unlockF();
}

Semaphore::~Semaphore(){
	if(myImpl) delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal(){
	myImpl->signal();
}

int Semaphore::val() const{
	return myImpl->vr;
}

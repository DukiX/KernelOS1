#include "event.h"
#include "kernelev.h"
#include "makroi.h"
#include "timer.h"

Event::Event(IVTno ivtNo){
	//Timer::lockF();
	myImpl = new KernelEv(ivtNo);
	//Timer::unlockF();
}

Event::~Event(){
	delete myImpl;
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}

#ifndef EVENT_H_
#define EVENT_H_

typedef unsigned char IVTno;

class KernelEv;

class Event{
public:
	Event(IVTno ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;
};



#endif /* EVENT_H_ */

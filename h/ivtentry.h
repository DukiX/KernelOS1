#ifndef IVTENTRY_H_
#define IVTENTRY_H_

typedef unsigned char IVTno;
typedef void interrupt (*rutina)(...);

class KernelEv;

class IVTEntry{
public:
	IVTEntry(IVTno br, rutina rut);
	~IVTEntry();

	KernelEv* kernelDog;

	rutina staraRutina;
	void pozoviStaru();

	static IVTEntry* uzmi(IVTno br);
private:
	IVTno brojUlaza;

	static IVTEntry* ivt[256];

};


#endif /* IVTENTRY_H_ */

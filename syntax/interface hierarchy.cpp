
#include <iostream>


#define pure  = 0


using namespace std;


struct IBase {
	virtual void DoSomething() pure;
};

struct IDerived : public IBase {
	virtual void DoAnything() pure;
};

class CBase : public IBase {
public:
	void DoSomething() override {
		cout << "CBase something.\n";
	}
};

class CDerived : public IDerived, public CBase {
public:
	void DoSomething() override {
		cout << "CDerived something.\n";
	}
	void DoAnything() override {
		cout << "CDerived anything.\n";
	}
};


int main() {
	IBase* base = new CBase();
	IDerived* derived = new CDerived();

	base->DoSomething();
	derived->DoSomething();
	derived->DoAnything();

	CBase* cbase = static_cast<CBase*>(base);
	CBase* cderived = static_cast<CBase*>(static_cast<IBase*>(derived));

	cbase->DoSomething();
	cderived->DoSomething();
}
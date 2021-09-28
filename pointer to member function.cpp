#pragma once

#include <unordered_map>

using namespace std;


struct Base {
	virtual void func() = 0;
};


struct Derived1 : Base {
	virtual void func() override {

	}
};


struct A {
	int val;
};

struct Derived2 : A, Base {
	virtual void func() override {

	}
};


struct Derived3 : A, virtual Base {
	virtual void func() override {

	}
};


struct Derived4 : Derived3, virtual Base {
	virtual void func() override {

	}
};


int main() {
	Base* p = nullptr;
	auto fp = &Base::func; sizeof(fp); //sizeof(Base::func);
	auto mfp = &Base::func; sizeof(mfp); sizeof(&Base::func);
	auto dfp = &Derived4::func; sizeof(dfp);

	void (Base:: * pf)() = &Base::func;

	//(p->*pf)();

	Derived1 d1;
	Derived2 d2;
	Derived3 d3;
	Derived4 d4;
}
// test.h

#pragma once

class Uncopyable {
protected:
	Uncopyable() = default;
	~Uncopyable() = default;
private:
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator=(const Uncopyable&) = delete;
};


class A : Uncopyable{
private:
	int a;
	A(int a) : a(a) {}
public:
	static A& Get();

	void f() {
		a++;
	}
};



// test.cpp

#include "A.h"

A& A::Get() {
	static A a(1);
	return a;
}

A& a = A::Get();



// main.cpp

#include "A.h"

extern A& a;

int main() {
	a.f();
}
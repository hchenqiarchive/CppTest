// template_test.h
#pragma once

#include <iostream>

using namespace std;

struct A {
	virtual void f() {
		cout << "A";
	}
};

template<class T>
struct B : A {
	virtual void f();
};


// template_test.cpp
#include "template_test.h"

template<class T>
void B<T>::f() {
	cout << "B";
}

template B<int>;
template B<float>;


// main.cpp
#include "template_test.h"

int main() {
	B<int> b1; 
	B<float> b2;
	//B<char> b3;  // link error

	A& a1 = b1;
	A& a2 = b2;
	a1.f(); 
	a2.f(); // (MSVC release version) calls the same function (but v_table is not shared)
}
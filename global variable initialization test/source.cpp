#include "A.h"

A& A::Get() {
	static A a(1);
	return a;
}


A& a = A::Get();
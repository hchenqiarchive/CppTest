

struct A {
	int m;
	int n;
	int f() { return m += 1; }
	int g(int c) { return m += c; }
};


void h(A& a, int A::*val) {
	a.*val = 2;
}

/*
pointer to members are only useful when there are multiple members with the same type,
this could also be implemented using arrays but this allows different members with distinct names,
or can be used in templates to reference different data members for different class type.
For example, it can be used in a serializer, or in a tree node with left and right pointer.
*/


int main() {
	A a;

	int A::*val = &A::m;
	int A::*val2 = &A::n;
	int (A::*func) () = &A::f;
	int (A::*gunc) (int) = &A::g;

	a.*val = 1;
	(a.*func)();
	(a.*gunc)(2);

	A* b = &a;
	b->*val = 1;
	(b->*func)();
	(b->*gunc)(2);

	h(a, val);
	h(a, val2);
}
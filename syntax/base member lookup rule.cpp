struct A {
	int m;
};

struct B : A {
	void m() {}
};


int main() {
	B b;
	b.m();  // calls B::m()
	b.A::m;  // returns A::m
}

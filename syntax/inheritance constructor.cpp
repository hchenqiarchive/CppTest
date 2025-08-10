class a {
public:
	a(int a1, int a2) {}

	void f(int a) { }
};

class b : public a {
public:
	using a::a;
	using a::f;

	// b(int b1, int b2) : a(b1, b2) {}

	void f() {}
};

int main() {
	b b(1, 2);
	b.f(1);
}

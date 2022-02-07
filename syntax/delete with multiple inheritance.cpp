#include <memory>

struct A {
	int a;
};

struct B {
	virtual ~B() {}  // heap corruption will occur without this virtual destructor
	int b;
};

struct C : A, B {
	int c;
};

int main() {
	//std::unique_ptr<A> pa = std::make_unique<C>();
	std::unique_ptr<B> pb = std::make_unique<C>();
}

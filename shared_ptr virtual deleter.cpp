#include <memory>

struct A {
	~A() {}
};

int main() {
	std::shared_ptr<A> a = std::make_shared<A>(); static_assert(sizeof(a) == 16);
	std::shared_ptr<void> b = a; a.reset();
	// A::~A() is called when b destructs
}
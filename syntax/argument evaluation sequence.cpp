#include <memory>
#include <iostream>
#include <typeinfo>

struct A {
	A() {
		std::cout << "A constructor" << std::endl;
		// throw 0;
	}
	~A() {

	}
};

struct B {
	B() {
		std::cout << "B constructor" << std::endl;
	}
	~B() {

	}
};

template<class T>
class ptr : public std::unique_ptr<T> {
public:
	ptr(T* t) : std::unique_ptr<T>(t) {
		std::cout << "ptr constructor of " << typeid(T).name() << std::endl;
	}
};

void f(ptr<A> a, ptr<B> b) {

}

int main() {
	f(new A, new B);
}


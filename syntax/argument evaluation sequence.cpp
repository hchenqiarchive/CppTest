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


// risk with variable number of arguments
#include <memory>
#include <vector>

struct A {
    A() {
        // might throw an exception here
    }
};

template<class T>
class ptr : public std::unique_ptr<T> {
public:
    ptr(T* t) : std::unique_ptr<T>(t) {}
};

void f(auto&& ...arg) {
    std::vector<ptr<A>> v;
    (v.emplace_back(std::move(arg)), ...);
}

int main() {
    f(new A, new A, new A);
}



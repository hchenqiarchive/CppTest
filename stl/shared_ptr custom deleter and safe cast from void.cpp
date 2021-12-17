#include <memory>
#include <iostream>


class A {
public:
	~A() {

	}
};


template<class T>
struct deleter {
	void operator()(T* object) { delete object; }
};


template<class T>
std::shared_ptr<T> dynamic_pointer_cast(const std::shared_ptr<void>& ptr) {
	if (std::get_deleter<deleter<T>>(ptr) != nullptr) {
		return std::static_pointer_cast<T>(ptr);
	}
	return nullptr;
}


int main() {
	std::shared_ptr<A> p(new A, deleter<A>());
	std::shared_ptr<void> pv = p; p.reset();

	p = dynamic_pointer_cast<A>(pv);
	std::cout << p;
	std::shared_ptr<int> p2 = dynamic_pointer_cast<int>(pv);
	std::cout << p2;
}
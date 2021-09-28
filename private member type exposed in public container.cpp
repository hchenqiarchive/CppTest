#include <vector>


struct A {
private:
	struct B {
		int b;
	};
public:
	std::vector<B> v = std::vector<B>(1);
};


int main() {
	A a;
	a.v[0].b = 0;  // ok
	//A::B b = a.v[0];  // error
	auto b = a.v[0];  // ok
	b.b = 0;
}
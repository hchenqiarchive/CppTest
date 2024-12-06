#include "type_traits"

class Base {

};

class Derived : public Base {

};

template<class T>
void f(T& object) {
// hit
}

void f(Base& object) {

}

template<class T> requires(!std::is_base_of_v<Base, T>)
void g(T& object) {

}

void g(Base& object) {
// hit
}

int main() {
	Derived d;
	f(d);
	g(d);
}

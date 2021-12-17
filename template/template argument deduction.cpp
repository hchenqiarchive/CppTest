#include <array>


template<size_t count>
class A {
public:
	template<class ...T>
	A(T... vals) : a{ vals... } {}
private:
	std::array<int, count> a;
};

template<class ...T>
A(T...)->A<sizeof...(T)>;

A()->A<10>;


int main() {
	A a{ 5, 3, 3, 1 };  // A<4>
	A b; 				// A<10>
}



// another example
#include <memory>

struct Vertical {};
struct Horizontal {};

template<class T>
class A;

template<>
class A<Vertical> {
public:
	A(std::unique_ptr<int> m) : m(std::move(m)) {}
private:
	std::unique_ptr<int> m;
};

template<>
class A<Horizontal> {
public:
	A(std::unique_ptr<float> m) : m(std::move(m)) {}
private:
	std::unique_ptr<float> m;
};

A(std::unique_ptr<int>)->A<Vertical>;
A(std::unique_ptr<float>)->A<Horizontal>;

int main() {
	A a(std::make_unique<int>());
	A b(std::make_unique<float>());
}



// another example with partial specialization
// another example
#include <memory>

struct Vertical {};
struct Horizontal {};

template<class T1, size_t size>
class A;

template<size_t size>
class A<Vertical, size> {
public:
	A(std::unique_ptr<int> m) : m(std::move(m)) {}
private:
	std::unique_ptr<int> m;
};

template<size_t size>
class A<Horizontal, size> {
public:
	A(std::unique_ptr<float> m) : m(std::move(m)) {}
private:
	std::unique_ptr<float> m;
};


//template<size_t size>
//using AV = A<Vertical, size>;

template<size_t size>
class AV : public A<Vertical, size> {
public:
	using A<Vertical, size>::A;
};

AV(std::unique_ptr<int>)->AV<3>;


int main() {
	AV a(std::make_unique<int>());
}
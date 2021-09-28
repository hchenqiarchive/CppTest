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


int main() {
	A a{ 5, 3, 3, 1 };
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
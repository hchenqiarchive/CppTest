#include <type_traits>
#include <memory>

class A {};

class B : public A {};
class C : public A {};
class D : public C {};
class E : private A {};

class F {};


// T must be derived class of A
//template<class T, typename std::enable_if<std::is_base_of<A, T>::value, int>::type = 0>
//class Test {};

// short version
template<class T, std::enable_if_t<std::is_base_of_v<A, T>, int> = 0>
class TestBase {};

template<class T, std::enable_if_t<std::is_convertible_v<T, A>, int> = 0>
class TestConvert {};

template<class T, std::enable_if_t<std::is_constructible_v<A, T>, int> = 0>
class TestConstruct {};

int main() {
	TestBase<A> tb1;	// ok
	TestBase<B> tb2;	// ok
	TestBase<C> tb3;	// ok
	TestBase<D> tb4;	// ok
	TestBase<E> tb5;	// ok
	//TestBase<F> tb6;		// error

	TestBase<A> tc1;	// ok
	TestBase<B> tc2;	// ok
	TestBase<C> tc3;	// ok
	TestBase<D> tc4;	// ok
	TestBase<E> tc5;	// ok
	//TestBase<F> tc6;		// error

	//TestConstruct<E> t5;	// error
	//TestConstruct<F> t6;	// error
}
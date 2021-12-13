#include <type_traits>


struct Assigned {};


struct A {
	using type = Assigned;
};


template<class T>
struct Ptr {
	template<class C, class = std::enable_if_t<std::is_same_v<T, typename C::type>>>
	Ptr(C a) {}
};


template<class T>
struct Frame {
	Frame(Ptr<T> ptr) {}
};


template<class C>
Frame(C)->Frame<typename C::type>;


int main() {
	auto frame1 = Frame<Assigned>(A());
	auto frame2 = Frame(A());  // ok, deduced Frame<Assigned>
}

#include <vector>
#include <map>
#include <unordered_set>


template<class T, class = void>
struct A {

};

//template<class T>
//struct A<std::vector<T>> {
//	static size_t size(const std::vector<T>& object) {
//		return object.size();
//	}
//};

template<template<class... Ts> class Container, class... Ts>
struct A<Container<Ts...>> {
	static size_t size(const Container<Ts...>& object) {
		return object.size();
	}
};


// The order does not matter.
template<class T>
struct A<std::vector<T>> {
	static size_t size(const std::vector<T>& object) {
		return object.size();
	}
};


template<class T>
size_t f(const T& a) {
	return A<T>::size(a);
}


int main() {
	std::vector<int> a = { 1,2,3 };
	std::map<int, char> b = { {1,'a'},  {2,'b'},  {3,'c'},  {4,'d'} };
	std::unordered_set<float> c = { 1.0, 2.0 };
	size_t size;
	size = f(a);
	size = f(b);
	size = f(c);
}
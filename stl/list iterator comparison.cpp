/*
Iterators from different list objects are disallowed to compare (at least in debug mode), 
  so when you want to store an iterator and check if it is initialized, 
  don't use comparison operators (==,!=), just check if _Ptr is nullptr.
*/

#include <list>

using namespace std;

struct A {
	list<A>::iterator index;
};

template<class T>
inline bool is_list_iterator_valid(typename std::list<T>::iterator it) { return it._Ptr != nullptr; }


/*
template<class T>
using list_iterator_type = typename list<T>::iterator;

template<class T>
struct list_iterator : list_iterator_type<T> {
	list_iterator(list_iterator_type<T> it) :
		list_iterator_type<T>(it) {
	}
	bool is_valid() const {
		return this->_Ptr != nullptr;
	}
};
*/

template<class T>
struct list_iterator : list<T>::iterator {
	list_iterator(typename list<T>::iterator it) : list<T>::iterator(it) {}
	bool valid() const { return this->_Ptr != nullptr; }
};

int main() {
	list<A> l;

	A a;
	bool ret = is_list_iterator_valid<A>(a.index);
	//ret = is_list_iterator_valid(a.index);  // error

	// list_iterator_type<A> it = a.index;

	ret = list_iterator<A>(l.begin()).valid();
	//ret = list_iterator<A>(l.begin()).is_valid();  // error
	//ret = list_iterator(l.begin()).is_valid();	 // error

	l.push_front(A{}); l.front().index = l.begin();
}
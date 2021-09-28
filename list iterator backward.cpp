// c++ list element swapping.

#include <list>

using namespace std;

// Swap the list element pointed by iterator with the next one,
//   without any element construction, copying or destruction.
// Usage:
//   list: a(begin) <-> b <-> c <-> d <-> (end)
//   $ list_iter_backword(list, b);
//   list: a(begin) <-> c <-> b <-> d <-> (end)
// Returns false if iterator points to the last element or the end.
template <class _Ty, class _Alloc>
bool list_iter_backword(list<_Ty, _Alloc>& _List, typename list<_Ty, _Alloc>::iterator& _Iter) {
	auto _Iter_Prev = _Iter;
	if (_Iter == _List.end() || ++_Iter == _List.end()) {
		return false; 
	}
	_List.splice(_Iter_Prev, _List, _Iter);
	return true;
}


int main() {
	list l = { 1,2,3,4,5 };
	list_iter_backword(l, ++l.begin());

	l.splice(++l.begin(), l, ++++l.begin(), ++++++++l.begin());
}



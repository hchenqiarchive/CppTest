// Must use list rather than forward_list if you want to store an iterator index.


#include <forward_list>

int main() {
	std::forward_list<int> l;
	//auto it0 = l.begin();
	//l.insert_after(it0, 0);  // Error!
	auto it = l.before_begin();
	it = l.insert_after(it, 1);
	it = l.insert_after(it, 2);
	it = l.insert_after(it, 3);
	l.erase_after(++l.before_begin());
}
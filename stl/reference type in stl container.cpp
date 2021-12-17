#include <list>
#include <map>
#include <cassert>

int main() {
	//std::list<int&> l;  // error: pointer to a reference is illegal
	std::map<int, int&> m;  // fine
	int a = 2;
	m.insert({ 1,a });
	m.begin()->second = 3;
	assert(a == 3);
}
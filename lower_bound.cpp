#include <algorithm>
#include <vector>

using namespace std;

struct A {
	int x;
	A(int x) : x(x) {}
};

const int number = 8947;


int main() {
	// Find the last index that v[index].x <= x.
	vector<A> v = { 0, 245, 483, 846 };
	auto cmp = [](const A& a, int x) { return a.x > x; };
	auto it = lower_bound(v.rbegin(), v.rend(), number, cmp);
	int index = v.rend() - it - 1;

	auto cmp2 = [](const A& a, int x) {return x >= a.x; };
	auto it2 = lower_bound(v.begin(), v.end(), number, cmp2);
	int index2 = it2 - v.begin() - 1;
}
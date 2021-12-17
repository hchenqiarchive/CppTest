struct {
public:
	const vector<int> content;
}

// Or use

struct {
private:
	vector<int> content;
public:
	const vector<int>& GetContent() const { return content; }
}

// Both provide a constant interface.
// They are slightly different. When the struct is moved, the default move constructor copys the first's content, but moves the second's content.



#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

struct A {
	const vector<int> a;
	const int b;
};

int main() {
	A a = { vector<int>{1,2,3}, 2 };
	A b = std::move(a);   // did not actually move because a is const

	auto pair = std::make_pair<int, const A&>(0, b);
}
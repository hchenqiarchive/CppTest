#include <string>


using namespace std;


struct A {
	mutable string str;

	string& get() & { return str; }
	const string& get() const& { return str; }
	string get() && { return std::move(str); }
	string get() const&& { return std::move(str); }

	// Error E2449: overloading two member functions with the same parameter types requires that they both have ref-qualifiers or both lack ref-qualifiers
	//string& get() { return str; }
	//string& get() const { return str; }
};


int main() {
	A a1; a1.get();
	const A a2; a2.get();
	A().get();
	const A a4; std::move(a4).get(); // Warning C26478: Don't use std::move on constant variables.
}
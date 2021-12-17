

template<int a>
struct B {
	void f() {}
};

template<int a>
struct C : public B<a> {
	//using B<a>::f;
	void g() { this->f(); }
};

int main() {
	C<1>().g();
}
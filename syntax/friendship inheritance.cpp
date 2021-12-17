
class A {
	friend class B;
	int m;  // [only accessible to class B and all its derived classes]
};


class B {
	void f(A& a) {
		a.m = 1;
	}
	virtual void v(A& a) {

	}
};

class C : public B {
	void g(A& a) {
		a.m = 2;  // Error!
	}
	virtual void v(A& a) override{
		a.m = 2;  // Error!
	}
};

template<class T>
class A2 {
	friend class T;
	int n;
};


class D : public B {
	void h(A2<D>& a) {
		a.n = 2;  // Fine.
	}
};

int main() {

}

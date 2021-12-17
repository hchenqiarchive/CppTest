
struct A {
	int a = 0;

	template<class T>
	void f(T& t) {
		a++;
		int i;
		short j;
		char k;
		f(i);
		f(j);
		f(k);
	}

	template<>
	void f<int>(int& i) {
		a--;
	}

	//template<>
	//void f<int>(int i) {  // error
	//	a--;
	//}

	template<>
	void f(short& i) {
		a--;
	}

	//template<>
	//void f(short i) {  // error
	//	a--;
	//}

	void f(char& i) {
		a--;
	}

	void f(char* i) {  // ok
		a--;
	}

	template<class T>
	void f(T a, int b) {

	}

	template<>
	void f<>(int a, int b) {

	}
};

int main() {
	A a;
	float f;
	int i;
	a.f(f);
	a.f(i);
}
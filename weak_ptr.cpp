#include <memory>

using namespace std;

struct A {
	~A() {

	}
};


int main() {
	shared_ptr<A> a = make_shared<A>();
	weak_ptr<A> wa = a;
	a.reset();
	shared_ptr<A> b = wa.lock();
}
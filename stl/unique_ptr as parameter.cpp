#include <memory>

using namespace std;


void f(unique_ptr<int> a) {

}


int main() {
	unique_ptr<int> a(new int(3));

	//f(new int(3)); // error
	f(unique_ptr<int>(new int(3))); // ok
	f(make_unique<int>(3)); // ok
}
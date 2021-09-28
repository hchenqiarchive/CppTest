#include <functional>
#include <iostream>


class Timer {
private:
	std::function<void(void)> on_alert;
public:
	Timer(std::function<void(void)> on_alert) : on_alert(on_alert) {}
	void OnAlert() { on_alert(); }
	void Set() {}
	void Stop() {}
};
static_assert(sizeof(Timer) == 64);


void f() {
	std::cout << "f() called" << std::endl;
}


class A {
public:
	void DoSomething() {
		std::cout << "A::DoSomething() called" << std::endl;
	}
	void DoSomethingElse(int val) {
		std::cout << "A::DoSomethingElse(" << val << ") called" << std::endl;
	}
};


class B {
private:
	Timer timer = Timer([&]() { f(); });
public:
	void g() { timer.Set(); }
	void f() { timer.Stop(); }
};


int main() {
	Timer timer1(f); timer1.OnAlert();
	Timer timer2([]() {std::cout << "[](){} called" << std::endl; }); timer2.OnAlert();

	A a;
	Timer timer3(std::bind(&A::DoSomething, &a)); timer3.OnAlert();
	Timer timer4(std::bind([&]() { a.DoSomething(); })); timer4.OnAlert();
	Timer timer5(std::bind(&A::DoSomethingElse, &a, 1)); timer5.OnAlert();

	B b;

}
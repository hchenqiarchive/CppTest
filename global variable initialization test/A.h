#pragma once

class Uncopyable {
protected:
	Uncopyable() = default;
	~Uncopyable() = default;
private:
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator=(const Uncopyable&) = delete;
};


class A : Uncopyable{
private:
	int a;
	A(int a) : a(a) {}
public:
	static A& Get();

	void f() {
		a++;
	}
};
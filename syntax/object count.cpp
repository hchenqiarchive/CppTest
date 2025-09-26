
template<class T>
class ObjectCount {
public:
	ObjectCount() {
		count++;
	}
	ObjectCount(const ObjectCount&) {
		count++;
	}
	~ObjectCount() {
		count--;
	}
private:
	static size_t count;
public:
	static size_t GetCount() { return count; }
};


class A : public ObjectCount<A> {
public:
	A() : A(0) {

	}
	A(int) {

	}
	~A() {

	}
};


size_t A::ObjectCount::count;


int main() {
	A a;
	{
		A b;
		A::GetCount();
	}
	A::GetCount();
	return 0;
}

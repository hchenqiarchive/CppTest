
template<class T>
class WeakPtr {
public:
	T* obj;
	bool alive;
public:
	WeakPtr() : obj(nullptr), alive(false) {}
	WeakPtr(T& obj) : obj(&obj), alive(true) {}
};


class ObjectPtr {
	bool* alive;
public:
	ObjectPtr(): alive(nullptr) {}
	ObjectPtr(const ObjectPtr&) = delete; // uncopyable
	void operator=(ObjectPtr&& right) noexcept { ObjectPtr temp; temp.alive = alive; alive = right.alive; right.alive = nullptr; }
	~ObjectPtr() { if (alive != nullptr) { *alive = false; } }
	template<class T>
	ObjectPtr(WeakPtr<T>& ptr) : alive(&ptr.alive) {}
};


struct Object {
	ObjectPtr liveliness;
};


struct Watcher {
	WeakPtr<Object> ptr;
};


int main() {
	Object* object = new Object;
	Watcher watcher;
	watcher.ptr = *object; object->liveliness = ObjectPtr(watcher.ptr);
	delete object;
}
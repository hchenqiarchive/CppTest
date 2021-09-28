
struct A;

struct B {
	B(A& a);
};

struct A {
	B b;
	A(B b): b(b) {}
	virtual void f() {}
};

struct C : public A {
	C() :A(B(*this)) {}
};

B::B(A& a) {
	a.f();
}

int main() {
	C c;  // runtime error.
}

// virtual function called before base class is constructed

// In release mode, it seems that the vtable is not set at all, see below.
// Compiler must have noticed the error(undefined behaviour) and did optimization.
//     22: int main() {
// 00007FF773861000 48 83 EC 48          sub         rsp,48h  
// 00007FF773861004 48 8B 05 FD 1F 00 00 mov         rax,qword ptr [__security_cookie (07FF773863008h)]  
// 00007FF77386100B 48 33 C4             xor         rax,rsp  
// 00007FF77386100E 48 89 44 24 30       mov         qword ptr [rsp+30h],rax  
//     23: 	C c;  // runtime error.
// 00007FF773861013 48 8B 44 24 20       mov         rax,qword ptr [c]  
// 00007FF773861018 48 8D 4C 24 20       lea         rcx,[c]  
// 00007FF77386101D FF 10                call        qword ptr [rax]  
//     24: 	// (virtual function called before base class is constructed)
//     25: }
// 00007FF77386101F 33 C0                xor         eax,eax  
// 00007FF773861021 48 8B 4C 24 30       mov         rcx,qword ptr [rsp+30h]  
// 00007FF773861026 48 33 CC             xor         rcx,rsp  
// 00007FF773861029 E8 22 00 00 00       call        __security_check_cookie (07FF773861050h)  
// 00007FF77386102E 48 83 C4 48          add         rsp,48h  
// 00007FF773861032 C3                   ret  
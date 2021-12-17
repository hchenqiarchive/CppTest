#include <vector>
#include <iostream>


template<class T>
constexpr size_t f(T& t) {
	return sizeof(T);
}


template<>
constexpr size_t f<std::vector<int>>(std::vector<int>& a) {
	size_t s = 0;
	for (auto& item : a) {
		s += f(item);
	}
	return s;
}


int main() {
	size_t s;
	std::cin >> s;
	std::vector<int> a;
	while (s--) { int m; std::cin >> m; a.push_back(m); }
	s = f(a);
	return 0;
}

/* x64 msvc v19.29 VS16.11 /O2
LN55@main:
		mov     rcx, QWORD PTR a$[rsp]
		mov     rax, rcx
		cmp     rcx, rbx
		je      SHORT $LN25@main
		npad    4
$LL26@main:
		add     rsi, 4
		add     rax, 4
		cmp     rax, rbx
		jne     SHORT $LL26@main
$LN25@main:

*/

/* x86-64 gcc 11.2
.L43:
		mov     rdi, QWORD PTR [rsp+16]
		mov     rdx, QWORD PTR [rsp+24]
		cmp     rdi, rdx
		je      .L27
		mov     rax, rdx
		sub     rax, rdi
.L27:
		mov     QWORD PTR [rsp+8], rax
*/

/* x86-64 clang 12.0.1
.LBB0_23:
		mov     rax, rbp
		sub     rax, r12
		and     rax, -4
		xor     ecx, ecx
		cmp     rbp, r12
		cmovne  rcx, rax
		mov     qword ptr [rsp + 16], rcx
*/
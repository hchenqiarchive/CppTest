
[[noreturn]] void f() {
	throw;
}
void g [[noreturn]] () {
	throw;
}

[[deprecated("no use")]] void h() {

}


[[bar]] int main() { // warning C5030: attribute 'bar' is not recognized
	switch (0) {
	case 0:
	case 1: [[fallthrough]];
	case 2:
	[[likely]] case 3:   // warning C5051: attribute 'likely' requires at least '/std:c++latest'; ignored
	default:
		break;
	}

	if (0 < 1) [[likely]] {

	} 

	if (0 >= 1) [[unlikely]] {

	}

	f();
	g();
	h();  // error C4996: 'h': no use

	[[foo]];  // warning C5030: attribute 'foo' is not recognized
}
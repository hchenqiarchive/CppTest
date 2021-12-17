#include <iostream>


static constexpr auto endl = '\n';


template<class Arg>
void print(std::ostream& out, Arg&& arg) {
	out << arg;
}

template<class Arg, class ...Args>
void print(std::ostream& out, Arg&& arg, Args&&... args) {
	out << arg;
	print(out, std::forward<Args>(args)...);
}


int main() {
	print(std::cout, "add ", "s0", "s1", "sa", endl);
}
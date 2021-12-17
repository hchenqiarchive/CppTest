#include <string>
#include <iostream>

using namespace std;

int main() {
	char s[27] = "abcdefghijklmnopqrstuvwxyz";
	string str = s;

	string_view view(s + 5, 5);
	string_view view2(str);
	string_view view3(view2.substr(5, 5));

	//view[0] = 'a';  // const

	cout 
		<< view << endl 
		<< view3 << endl;
}
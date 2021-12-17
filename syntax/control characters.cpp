#include <locale>
#include <string>
#include <algorithm>


using std::wstring;
using wchar = wchar_t;


// filter out control characters /*and invalid surrogate pairs*/
void FilterString(wstring& str) { std::erase_if(str, iswcntrl); }


int main() {
	wchar a[] = L"\U0001F60A";
	wchar rtl = L'\u202e';
	bool res = iswcntrl(rtl); //  U'\x1f60a'

	wstring str = L"\u202d\U0001F60A\u202e\u203e\n3";
	FilterString(str);
}
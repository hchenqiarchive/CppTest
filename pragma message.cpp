
#define __ToString(name) #name
#define _ToString(name) __ToString(name)
#define _LINE _ToString(__LINE__)
#define REVIEW __FILE__ "(" _LINE "): "
#define Remark(something)	__pragma(message(__FILE__ "(" _LINE "): [" __FUNCTION__ "] Remark: " #something))


void GetDisplayRegion() {
	Remark(There should be another way);
}

int main() {
#pragma message(REVIEW"There should be another way to implement this.")
}

/* Output: 
Build started...
1>------ Build started: Project: SimpleTest, Configuration: Debug x64 ------
1>main.cpp
1>E:\source\SimpleTest\SimpleTest\main.cpp(10): [GetDisplayRegion] Remark: There should be another way
1>E:\source\SimpleTest\SimpleTest\main.cpp(14): There should be another way to implement this.
1>SimpleTest.vcxproj -> E:\source\SimpleTest\build\x64\Debug\SimpleTest.exe
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
*/
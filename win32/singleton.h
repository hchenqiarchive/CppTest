#pragma once

#include <Windows.h>

#include <stdexcept>
#include <string>


void EnsureSingleInstance(std::wstring str) {
	HANDLE mutex = CreateMutexW(NULL, FALSE, str.c_str());
	if (GetLastError() == ERROR_ALREADY_EXISTS || GetLastError() == ERROR_ACCESS_DENIED) {
		throw std::runtime_error("another instance is already running");
	}
}

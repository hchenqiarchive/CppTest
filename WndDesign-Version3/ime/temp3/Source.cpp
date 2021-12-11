
#include "ime_input.h"

#include <Windows.h>

HWND hWnd;

IMM32Manager ime;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	const wchar_t className[] = L"ImeTestClass";
	const wchar_t titleName[] = L"ImeTest";

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = className;

	if (!RegisterClassEx(&wc)) { return 0; }

	hWnd = CreateWindowEx(NULL,
						  className, titleName,
						  WS_OVERLAPPEDWINDOW,
						  200, 200, 800, 500,
						  NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) { return 0; }

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static wstring result = L"result", composition = L"composition";
	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOutW(hdc, 50, 50, result.data(), result.length());
		TextOutW(hdc, 50, 100, composition.data(), composition.length());
		EndPaint(hwnd, &ps);
		break;

	case WM_RBUTTONDOWN:
		ime.DisableIME(hwnd);
		break;
	case WM_MBUTTONDOWN:
		ime.EnableIME(hwnd);
		break;

	case WM_IME_SETCONTEXT:
		ime.CleanupComposition(hwnd);
		ime.UpdateCaretRect(hwnd, Rect(100, 100, 100, 100));
		ime.is_composing = false;
		break;
	case WM_IME_STARTCOMPOSITION:
		ime.UpdateCaretRect(hwnd, Rect(100, 100, 100, 100));
		break;
	case WM_IME_COMPOSITION:
		ime.is_composing = true;
		ime.GetResult(hwnd, lParam, result);
		ime.GetComposition(hwnd, lParam, composition);
		InvalidateRect(hwnd, NULL, true);
		break;
	case WM_IME_ENDCOMPOSITION:
		ime.is_composing = false;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
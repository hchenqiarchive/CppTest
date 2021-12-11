#include <string>

#include <Windows.h>


using uint = unsigned int;
using wchar = wchar_t;
using Point = struct { int x; int y; };

using std::wstring;


#pragma comment(lib, "imm32.lib")


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

	HWND hWnd = CreateWindowEx(NULL, className, titleName, WS_OVERLAPPEDWINDOW,
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


void ImeEnable(HWND hwnd) {
	ImmAssociateContextEx(hwnd, nullptr, IACE_DEFAULT);
}

void ImeDisable(HWND hwnd) {
	ImmAssociateContextEx(hwnd, nullptr, 0);
}

void ImeCancle(HWND hwnd) {
	HIMC imc = ImmGetContext(hwnd);
	ImmNotifyIME(imc, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
	ImmReleaseContext(hwnd, imc);
}

void ImeSetPosition(HWND hwnd, Point point) {
	HIMC imc = ImmGetContext(hwnd);
	COMPOSITIONFORM cf = { CFS_POINT, { point.x, point.y } };
	ImmSetCompositionWindow(imc, &cf);
	ImmReleaseContext(hwnd, imc);
}

void ImeGetString(HWND hwnd, uint type, wstring& result) {
	if (type & GCS_RESULTSTR) { type = GCS_RESULTSTR; } //
	else if (type & GCS_COMPSTR) { type = GCS_COMPSTR; } //
	else { return result.clear(); }
	HIMC imc = ImmGetContext(hwnd);
	uint size = ImmGetCompositionStringW(imc, type, nullptr, 0);
	result.resize(size / sizeof(wchar));
	ImmGetCompositionStringW(imc, type, result.data(), size);
	ImmReleaseContext(hwnd, imc);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static wstring result = L"result";
	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOutW(hdc, 100, 100, result.data(), result.length());
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		ImeCancle(hwnd);
		break;
	case WM_RBUTTONDOWN:
		ImeDisable(hwnd);
		break;
	case WM_MBUTTONDOWN:
		ImeEnable(hwnd);
		break;

	case WM_IME_SETCONTEXT:
		break;
	case WM_IME_STARTCOMPOSITION:
		ImeSetPosition(hwnd, { 100, 100 });
		break;
	case WM_IME_COMPOSITION:
		ImeGetString(hwnd, lParam, result);
		InvalidateRect(hwnd, NULL, true);
		break;
	case WM_IME_ENDCOMPOSITION:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
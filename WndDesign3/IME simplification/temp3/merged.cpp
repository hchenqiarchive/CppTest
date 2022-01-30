
#include <string>

#include <Windows.h>

using Point = struct { int x; int y; };
using Size = struct { int width; int height; };
using Rect = struct { Point point; Size size; };

using uint = unsigned;

Rect region_empty = {};

using std::wstring;


class IMM32Manager {
public:
	IMM32Manager() : is_composing(false) {}
	~IMM32Manager() {}

	bool is_composing;

	void CleanupComposition(HWND hwnd);

	void EnableIME(HWND hwnd);
	void DisableIME(HWND hwnd);
	void CancelIME(HWND hwnd);

	void UpdateCaretRect(HWND hwnd, Rect caret_rect);

	void GetString(HIMC imm_context, WPARAM lparam, int type, wstring& result);

	void GetResult(HWND hwnd, LPARAM lparam, wstring& result);
	void GetComposition(HWND hwnd, LPARAM lparam, wstring& result);
};


#include <Windows.h>

HWND hWnd;

IMM32Manager ime;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



int main() {
	HINSTANCE hInstance = GetModuleHandle(NULL);
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

	ShowWindow(hWnd, SW_SHOW);
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
		ime.UpdateCaretRect(hwnd, Rect{ 100, 100, 100, 100 });
		ime.is_composing = false;
		break;
	case WM_IME_STARTCOMPOSITION:
		ime.UpdateCaretRect(hwnd, Rect{ 100, 100, 100, 100 });
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


#pragma comment(lib, "imm32.lib")
#include <stdexcept>

class ImmContext {
private:
	HWND hwnd;
	HIMC imm_context;
public:
	ImmContext(HWND hwnd) : hwnd(hwnd), imm_context(ImmGetContext(hwnd)) {
		if (imm_context == nullptr) { throw std::runtime_error("invalid imm context"); }
	}
	~ImmContext() {
		ImmReleaseContext(hwnd, imm_context);
	}
	operator HIMC () const { return imm_context; }
};


void IMM32Manager::UpdateCaretRect(HWND hwnd, Rect caret_rect) {
	if (GetFocus() != hwnd)return;

	ImmContext imm_context(hwnd);

	int x = caret_rect.point.x;
	int y = caret_rect.point.y;

	COMPOSITIONFORM cf = { CFS_POINT, {x, y} };
	ImmSetCompositionWindow(imm_context, &cf);
}

void IMM32Manager::CleanupComposition(HWND hwnd) {
	if (is_composing) {
		ImmContext imm_context(hwnd);
		ImmNotifyIME(imm_context, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
		is_composing = false;
	}
}

void IMM32Manager::GetString(HIMC imm_context, WPARAM lparam, int type, wstring& result) {
	if (!(lparam & type)) return result.clear();
	LONG string_size = ImmGetCompositionStringW(imm_context, type, NULL, 0);
	if (string_size <= 0 || string_size % sizeof(wchar_t) != 0) return result.clear();
	result.resize(string_size / sizeof(wchar_t));
	ImmGetCompositionStringW(imm_context, type, const_cast<wchar_t*>(result.data()), string_size);
}

void IMM32Manager::GetResult(HWND hwnd, LPARAM lparam, wstring& result) {
	ImmContext imm_context(hwnd);
	GetString(imm_context, lparam, GCS_RESULTSTR, result);
}

void IMM32Manager::GetComposition(HWND hwnd, LPARAM lparam, wstring& result) {
	ImmContext imm_context(hwnd);
	GetString(imm_context, lparam, GCS_COMPSTR, result);
}

void IMM32Manager::DisableIME(HWND hwnd) {
	CleanupComposition(hwnd);
	ImmAssociateContextEx(hwnd, NULL, 0);
}

void IMM32Manager::CancelIME(HWND hwnd) {
	if (is_composing) {
		ImmContext imm_context(hwnd);
		ImmNotifyIME(imm_context, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		is_composing = false;
	}
}

void IMM32Manager::EnableIME(HWND hwnd) {
	ImmAssociateContextEx(hwnd, NULL, IACE_DEFAULT);
}

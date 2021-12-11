#pragma once

#include <Windows.h>
#include <dwmapi.h>


#pragma comment(lib, "dwmapi.lib")


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	const wchar_t className[] = L"Win32TestWndClass";
	const wchar_t titleName[] = L"Win32Test";

	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = className;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	if (!RegisterClassExW(&wcex)) { return 0; }

	HWND hWnd = CreateWindowExW(NULL, //WS_EX_NOREDIRECTIONBITMAP,
								className, titleName,
								WS_OVERLAPPED | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_HSCROLL | WS_VSCROLL,
								200, 200, 800, 500,
								NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) { return 0; }

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE: {
		//const MARGINS shadow_on = { 1, 1, 1, 1 };
		//const MARGINS shadow_off = { 0, 0, 0, 0 };
		//DwmExtendFrameIntoClientArea(hwnd, &shadow_on);
	}break;

	//case WM_PAINT:
	case WM_NCPAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT rect; GetWindowRect(hwnd, &rect); rect.right -= rect.left; rect.bottom -= rect.top; rect.left = 0; rect.top = 0;
		RECT rect2 = rect; rect2.right -= 5; rect2.bottom -= 5; rect2.left = 5; rect2.top = 5;
		HBRUSH brush = CreateSolidBrush(0x000000FF);
		HBRUSH brush2 = CreateSolidBrush(0x0000FF00);
		FillRect(hdc, &rect, brush); 
		FillRect(hdc, &rect2, brush2);
		EndPaint(hwnd, &ps);
		DeleteObject(brush);
		DeleteObject(brush2);
	}break;

	case WM_GETMINMAXINFO: {
		MINMAXINFO* min_max_info = reinterpret_cast<MINMAXINFO*>(lparam);
		min_max_info->ptMaxPosition = { -5, -5 };
		min_max_info->ptMaxSize = { 1376, 738 };
		min_max_info->ptMinTrackSize = { 0, 0 };
		min_max_info->ptMaxTrackSize = { 1376, 778 };
	}break;
	case WM_WINDOWPOSCHANGING:
	case WM_WINDOWPOSCHANGED: {
		WINDOWPOS* position = reinterpret_cast<WINDOWPOS*>(lparam);
		if ((position->flags & SWP_NOSIZE) && (position->flags & SWP_NOMOVE)) { break; }  // Filter out other messages.
		
	}break;
	case WM_SETTINGCHANGE:
		break;

		// Intercept all non-client messages.
	case WM_NCCALCSIZE: break;
	case WM_NCACTIVATE: return true;
	case WM_NCHITTEST: return HTCLIENT;
	//case WM_NCPAINT: {
	//	HDC hdc;
	//	hdc = GetDCEx(hwnd, (HRGN)wparam, DCX_WINDOW | DCX_INTERSECTRGN);
	//	// Paint into this DC 
	//	ReleaseDC(hwnd, hdc);
	//}break;

	case WM_LBUTTONDOWN:
		SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, lparam);
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
	return 0;
}
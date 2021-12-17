#pragma once

#include <Windows.h>
#include <dwmapi.h>


#pragma comment(lib, "dwmapi.lib")


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	const wchar_t className[] = L"Win32TestWndClass";
	const wchar_t titleName[] = L"Win32Test";

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = className;
	if (!RegisterClassExW(&wc)) { return 0; }

	HWND hWnd = CreateWindowExW(NULL,//WS_EX_LAYERED, //
								className, titleName,
								WS_OVERLAPPEDWINDOW,
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE: {

		//DWM_BLURBEHIND bb = { 0 };
		//HRGN hRgn = CreateRectRgn(0, 0, -1, -1); //应用毛玻璃的矩形范围，
		////参数(0,0,-1,-1)可以让整个窗口客户区变成透明的，而鼠标是可以捕获到透明的区域
		//bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
		//bb.hRgnBlur = hRgn;
		//bb.fEnable = TRUE;
		//DwmEnableBlurBehindWindow(hWnd, &bb);

	}
		//break;
	case WM_DWMCOMPOSITIONCHANGED: {
		//MARGINS margins = { -1 };
		//// Extend the frame across the whole window.
		//DwmExtendFrameIntoClientArea(hWnd, &margins);

	}break;

	case WM_WINDOWPOSCHANGING:
	case WM_WINDOWPOSCHANGED: {
		WINDOWPOS* position = reinterpret_cast<WINDOWPOS*>(lParam);
		if ((position->flags & SWP_NOSIZE) && (position->flags & SWP_NOMOVE)) { break; }  // Filter out other messages.
		break;
	}

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

		// Intercept all non-client messages.
	//case WM_NCCALCSIZE:
	//	break;
	//case WM_NCACTIVATE:  // Do not draw the nonclient area.
	//	return true;
	//case WM_NCHITTEST:   // There's no non-client region.
	//	return HTCLIENT;
	//case WM_NCPAINT: 
	//	break;

	case WM_LBUTTONDOWN:
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
	return 0;
}
#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")


HWND hWnd;


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	const wchar_t className[] = L"Win32TestWndClass";
	const wchar_t titleName[] = L"Win32Test";

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = className;

	if (!RegisterClassExW(&wc)) { throw; }

	hWnd = CreateWindowExW(NULL,//WS_EX_LAYERED,
						   className, titleName,
						   WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX //| WS_OVERLAPPEDWINDOW //
						   | WS_HSCROLL | WS_VSCROLL,  // For horizontal scrolling.
						   200, 200, 800, 500,
						   NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) { throw; }


	//SetClassLongPtr(hWnd, GCLP_HICON, (LONG_PTR)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
	//SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);


	ShowWindow(hWnd, SW_SHOWDEFAULT);
	//ShowWindow(hWnd, SW_MAXIMIZE);
	//ShowWindow(hWnd, SW_MINIMIZE);
	//ShowWindow(hWnd, SW_HIDE);

	MSG msg;

	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

		// Intercept all non-client messages.
	case WM_NCCALCSIZE:
		InflateRect((LPRECT)lParam, 100, 100);  // Shrink the client region for border.
		break;
	case WM_NCACTIVATE:  // Do not draw the nonclient area.
		return true;
	case WM_NCHITTEST:   // There's no non-client region.
		return HTCLIENT;
	//case WM_NCPAINT: 
	//	break;

	case WM_GETMINMAXINFO:
		{
			MINMAXINFO* min_max_info = reinterpret_cast<MINMAXINFO*>(lParam);
			break;
		}
	case WM_WINDOWPOSCHANGED: 
		{
			WINDOWPOS* position = reinterpret_cast<WINDOWPOS*>(lParam);
			if ((position->flags & SWP_NOSIZE) && (position->flags & SWP_NOMOVE)) { break; }  // Filter out other messages.
			break;
		}

	case WM_RBUTTONDOWN: 
		{
			LONG style = GetWindowLong(hWnd, GWL_EXSTYLE);
			SetWindowLong(hWnd, GWL_EXSTYLE, style | WS_EX_TRANSPARENT | WS_EX_LAYERED);
			SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
			break;
		}
		{
			MARGINS margins = { 20,20,20,20 };
			HRESULT hr = S_OK;
			hr = DwmExtendFrameIntoClientArea(hWnd, &margins);
			break;
		}
	case WM_LBUTTONDOWN:  // Send the fake mouse messages hit on caption to fool DefWindowProc().
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
	return 0;
}

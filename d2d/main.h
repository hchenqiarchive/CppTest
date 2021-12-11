#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <dwmapi.h>

#include <future>

#pragma comment(lib, "dwmapi.lib")

HWND hWnd;

extern void Init();
extern void Final();
extern void OnPaint();
extern void OnSize(int, int);
extern void OnWheel(int, bool ctrl, int x, int y);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	const wchar_t className[] = L"D2DTestWndClass";
	const wchar_t titleName[] = L"D2DTest";

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	//wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // 不加的话如果窗口大小变化会出现黑色
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = className;

	if (!RegisterClassExW(&wc)) { throw; }

	hWnd = CreateWindowExW(WS_EX_LAYERED, //WS_EX_NOREDIRECTIONBITMAP,//
						   className, titleName,
						   WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX //| WS_OVERLAPPEDWINDOW //
						   | WS_HSCROLL | WS_VSCROLL,  // For horizontal scrolling.
						   200, 200, 800, 500,
						   NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) { throw; }

	Init();

	ShowWindow(hWnd, SW_SHOW);
	OnPaint();  // WM_PAINT will never be sent when WS_EX_LAYERED is used.

	MSG msg;

	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	Final();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		OnPaint();
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		if (::hWnd == nullptr) { break; }  // If use WS_POPUP rather than WS_OVERLAPPEDWINDOW, 
										   //   WM_SIZE will be sent before window is created.
		OnSize(LOWORD(lParam), HIWORD(lParam));
		OnPaint();
		break;

		// Intercept all non-client messages.
	case WM_NCCALCSIZE: 
		break;
	case WM_NCACTIVATE:  // Do not draw the nonclient area.
		return true;
		break;
	case WM_NCHITTEST:   // There's no non-client region.
		return HTCLIENT;
	//case WM_NCPAINT:
	//	break;

	case WM_ERASEBKGND: 
		return false;

	case WM_GETMINMAXINFO:
		{
			MINMAXINFO* min_max_info = reinterpret_cast<MINMAXINFO*>(lParam);
			break; 
		}
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:  // Send the fake mouse messages hit on caption to fool DefWindowProc().
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
		break;
	case WM_MOUSEWHEEL: {
			RECT rect;
			GetWindowRect(hWnd, &rect);
			OnWheel(
				GET_WHEEL_DELTA_WPARAM(wParam),
				(GET_KEYSTATE_WPARAM(wParam) & MK_CONTROL) != 0,
				GET_X_LPARAM(lParam) - rect.left, GET_Y_LPARAM(lParam) - rect.top
			);
		}break;

	case WM_RBUTTONDOWN:


		//{
		//	DWORD style = GetWindowLong(hWnd, GWL_EXSTYLE);
		//	style |= WS_EX_TOOLWINDOW;
		//	//style &= ~WS_EX_APPWINDOW;
		//	SetWindowLong(hWnd, GWL_EXSTYLE, style);
		//	SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED);
		//	break;
		//}
		//{
		//	//SetActiveWindow(hWnd);
		//	//SetFocus(hWnd);
		//	static std::future<BOOL> fu;
		//	fu = std::async(AnimateWindow, hWnd, 5000, AW_HIDE | AW_BLEND);
		//	SetTimer(hWnd, 123, 3000, NULL);
		//	break;
		//}
	case WM_TIMER:
		//ShowWindow(hWnd, SW_SHOW);
		AnimateWindow(hWnd, 2000, AW_BLEND);
		KillTimer(hWnd, 123);
		break;

	case WM_SETFOCUS:
		break;
	case WM_KILLFOCUS:
		break;
	case WM_ACTIVATE:
		break;

	case WM_MBUTTONDOWN:
		SetCapture(hWnd);
		break;
	case WM_CAPTURECHANGED:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
	return 0;
}

#include <Windows.h>
//#include <Dwmapi.h>

int __stdcall wWinMain(HINSTANCE module, HINSTANCE, PWSTR, int) {
	WNDCLASS wc = {};
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hInstance = module;
	wc.lpszClassName = L"window";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = [](HWND window, UINT message, WPARAM wparam, LPARAM lparam) -> LRESULT {
		if (WM_CREATE == message) {
			//DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
			//DwmSetWindowAttribute(window, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));
			SetWindowRgn(window, CreateRoundRectRgn(10, 10, 790, 490, 20, 20), true);
		}
		if (WM_DESTROY == message) { PostQuitMessage(0); return 0; }
		return DefWindowProc(window, message, wparam, lparam);
	};

	RegisterClass(&wc);

	HWND window = CreateWindowEx(NULL, //WS_EX_NOREDIRECTIONBITMAP, //
								 wc.lpszClassName, L"Sample",
								 WS_OVERLAPPEDWINDOW | WS_VISIBLE, //WS_POPUP, //
								 200, 200, 800, 500, //CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								 nullptr, nullptr, module, nullptr);
	ShowWindow(window, SW_SHOWDEFAULT);
	//ShowWindow(window, SW_MINIMIZE);
	//ShowWindow(window, SW_MAXIMIZE);
	MSG message;
	while (BOOL result = GetMessage(&message, 0, 0, 0)) {
		if (-1 != result) DispatchMessage(&message);
	}
}
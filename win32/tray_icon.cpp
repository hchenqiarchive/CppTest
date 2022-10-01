#include <Windows.h>


UINT WM_TASKBAR_CREATE = RegisterWindowMessage(L"TaskbarCreated");

NOTIFYICONDATA icon_data = {};


void TrayIconInitialize(HWND hWnd) {
	icon_data.cbSize = sizeof(NOTIFYICONDATA);
	icon_data.hWnd = hWnd;
	icon_data.uID = 0;
	icon_data.uCallbackMessage = WM_USER;
	icon_data.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcscpy_s(icon_data.szTip, L"Hello world!");
	icon_data.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
}

void TrayIconAdd() { Shell_NotifyIcon(NIM_ADD, &icon_data); }
void TrayIconUpdate() { Shell_NotifyIcon(NIM_MODIFY, &icon_data); }
void TrayIconRemove() { Shell_NotifyIcon(NIM_DELETE, &icon_data); }


int main() {
	WNDCLASS wc = {};
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"Test Tray";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = [](HWND window, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
		if (msg == WM_TASKBAR_CREATE) { TrayIconAdd(); }
		switch (msg) {
		case WM_USER:
			switch (LOWORD(lparam)) {
			case WM_RBUTTONDOWN: DestroyWindow(window); break;
			}
			break;
		case WM_DESTROY: TrayIconRemove(); PostQuitMessage(0); break;
		default:
		return DefWindowProc(window, msg, wparam, lparam);
		}
	};
	RegisterClass(&wc);

	HWND window = CreateWindowEx(NULL, wc.lpszClassName, L"Test Tray Window", WS_POPUP,
								 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								 NULL, NULL, wc.hInstance, NULL);

	TrayIconInitialize(window);
	TrayIconAdd();

	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

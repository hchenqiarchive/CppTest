#include "win32_ime_input.h"


LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    HDC hdc;
    PAINTSTRUCT ps;
    static wstring text = L"default";
    switch (msg) {
    case WM_LBUTTONDOWN:
        SetFocus(hwnd);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        TextOut(hdc, 50, 100, text.data(), text.length());
        EndPaint(hwnd, &ps);
        break;
    case WM_IME_SETCONTEXT:
        ime.CreateImeWindow(hwnd);
        ime.CleanupComposition(hwnd);
        ime.EnableIME(hwnd);
        //ime.SetImeWindowStyle(hwnd, msg, wparam, lparam);
        break;
    case WM_INPUTLANGCHANGE:
        ime.UpdateInputLanguage();
        break;
    case WM_IME_STARTCOMPOSITION:
        ime.CreateImeWindow(hwnd);
        ime.ResetComposition();
        //frame->ReceiveMessage(Msg::ImeCompositionBegin, nullmsg);
        break;
    case WM_IME_COMPOSITION:
        ime.UpdateImeWindow(hwnd);
        if (ime.UpdateComposition(hwnd, lparam)) {
            text = ime.GetComposition();
            hdc = GetWindowDC(hwnd);
            TextOut(hdc, 50, 100, text.data(), text.length());
            ReleaseDC(hwnd, hdc);
            //frame->ReceiveMessage(Msg::ImeComposition, ime_composition_msg);
        }
        ime.UpdateResult(hwnd, lparam);
        break;
    case WM_IME_ENDCOMPOSITION:
        //frame->ReceiveMessage(Msg::ImeCompositionEnd, ime_composition_msg);
        ime.ResetComposition();
        ime.DestroyImeWindow(hwnd);
        break;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    default: return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
};


int __stdcall wWinMain(HINSTANCE module, HINSTANCE, PWSTR, int) {
	WNDCLASS wc = {};
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hInstance = module;
	wc.lpszClassName = L"window";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;

	RegisterClass(&wc);

	HWND window = CreateWindowEx(NULL, //WS_EX_NOREDIRECTIONBITMAP,
								 wc.lpszClassName, L"Sample",
								 WS_OVERLAPPEDWINDOW | WS_VISIBLE, //WS_POPUP, //
								 200, 200, 800, 500, //CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								 nullptr, nullptr, module, nullptr);
	ShowWindow(window, SW_SHOWDEFAULT);
	MSG message;
	while (BOOL result = GetMessage(&message, 0, 0, 0)) {
		if (-1 != result) DispatchMessage(&message);
	}
}

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
	bool is_composing() const { return is_composing_; }

	IMM32Manager()
		: is_composing_(false),
		input_language_id_(LANG_USER_DEFAULT),
		system_caret_(false),
		caret_rect_{ -1, -1, 0, 0 },
		use_composition_window_(false) {
	}

	~IMM32Manager() {}

	void SetInputLanguage();

	void CreateImeWindow(HWND window_handle);

	LRESULT SetImeWindowStyle(HWND window_handle, UINT message,
							  WPARAM wparam, LPARAM lparam);

	void DestroyImeWindow(HWND window_handle);

	void UpdateImeWindow(HWND window_handle);

	void CleanupComposition(HWND window_handle);

	void ResetComposition(HWND window_handle);

	void GetResult(HWND window_handle, LPARAM lparam, wstring& result);

	void GetComposition(HWND window_handle, LPARAM lparam, wstring& result);

	void EnableIME(HWND window_handle);

	void DisableIME(HWND window_handle);
	void CancelIME(HWND window_handle);

	void UpdateCaretRect(HWND window_handle, Rect caret_rect);

	void SetUseCompositionWindow(bool use_composition_window);

	LANGID input_language_id() const { return input_language_id_; }

	bool IsInputLanguageCJK() const;

	bool IsImm32ImeActive();

protected:
	void MoveImeWindow(HWND window_handle, HIMC imm_context);

	void CompleteComposition(HWND window_handle, HIMC imm_context);

	bool GetString(HIMC imm_context, WPARAM lparam, int type, wstring& result);

private:
	bool is_composing_;

	LANGID input_language_id_;

	bool system_caret_;

	Rect caret_rect_;

	bool use_composition_window_;
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

	case WM_IME_SETCONTEXT:
		ime.CreateImeWindow(hwnd);
		ime.CleanupComposition(hwnd);
		ime.SetImeWindowStyle(hwnd, msg, wParam, lParam);
		ime.SetUseCompositionWindow(true);
		break;
	case WM_IME_STARTCOMPOSITION:
		ime.CreateImeWindow(hwnd);
		ime.ResetComposition(hwnd);
		ime.UpdateCaretRect(hwnd, Rect{ 100, 100, 100, 100 });
		break;
	case WM_IME_COMPOSITION: {
		ime.UpdateImeWindow(hwnd);
		ime.GetResult(hwnd, lParam, result);
		ime.GetComposition(hwnd, lParam, composition);
		InvalidateRect(hwnd, NULL, true);
	}break;
	case WM_IME_ENDCOMPOSITION:
		ime.ResetComposition(hwnd);
		ime.DestroyImeWindow(hwnd);
		break;
	case WM_INPUTLANGCHANGE:
		ime.SetInputLanguage();
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


void IMM32Manager::SetInputLanguage() {
	WCHAR keyboard_layout[KL_NAMELENGTH];
	if (::GetKeyboardLayoutNameW(keyboard_layout)) {
		input_language_id_ =
			static_cast<LANGID>(
				wcstol(&keyboard_layout[KL_NAMELENGTH >> 1], nullptr, 16));
	} else {
		input_language_id_ = 0x0409;  // Fallback to en-US.
	}
}

void IMM32Manager::CreateImeWindow(HWND window_handle) {
	if (PRIMARYLANGID(input_language_id_) == LANG_CHINESE ||
		PRIMARYLANGID(input_language_id_) == LANG_JAPANESE) {
		if (!system_caret_) {
			if (::CreateCaret(window_handle, NULL, 1, 1)) {
				system_caret_ = true;
			}
		}
	}
	UpdateImeWindow(window_handle);
}

LRESULT IMM32Manager::SetImeWindowStyle(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam) {
	lparam &= ~ISC_SHOWUICOMPOSITIONWINDOW;
	return ::DefWindowProc(window_handle, message, wparam, lparam);
}

void IMM32Manager::DestroyImeWindow(HWND window_handle) {
	// Destroy the system caret if we have created for this IME input context.
	if (system_caret_) {
		::DestroyCaret();
		system_caret_ = false;
	}
}

void IMM32Manager::MoveImeWindow(HWND window_handle, HIMC imm_context) {
	if (GetFocus() != window_handle)
		return;

	int x = caret_rect_.point.x;
	int y = caret_rect_.point.y;

	const int kCaretMargin = 1;
	if (!use_composition_window_ &&
		PRIMARYLANGID(input_language_id_) == LANG_CHINESE) {
		CANDIDATEFORM candidate_position = { 0, CFS_CANDIDATEPOS, {x, y},
											{0, 0, 0, 0} };
		::ImmSetCandidateWindow(imm_context, &candidate_position);
	}
	if (system_caret_) {
		switch (PRIMARYLANGID(input_language_id_)) {
		case LANG_JAPANESE:
			::SetCaretPos(x, y + caret_rect_.size.height);
			break;
		default:
			::SetCaretPos(x, y);
			break;
		}
	}
	if (use_composition_window_) {
		COMPOSITIONFORM cf = { CFS_POINT, {x, y} };
		::ImmSetCompositionWindow(imm_context, &cf);
		return;
	}

	if (PRIMARYLANGID(input_language_id_) == LANG_KOREAN) {
		y += kCaretMargin;
	}
	CANDIDATEFORM exclude_rectangle = { 0, CFS_EXCLUDE, {x, y},
		{x, y, x + caret_rect_.size.width, y + caret_rect_.size.height} };
	::ImmSetCandidateWindow(imm_context, &exclude_rectangle);
}

void IMM32Manager::UpdateImeWindow(HWND window_handle) {
	if (caret_rect_.point.x >= 0 && caret_rect_.point.y >= 0) {
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			MoveImeWindow(window_handle, imm_context);
			::ImmReleaseContext(window_handle, imm_context);
		}
	}
}

void IMM32Manager::CleanupComposition(HWND window_handle) {
	if (is_composing_) {
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			::ImmNotifyIME(imm_context, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
			::ImmReleaseContext(window_handle, imm_context);
		}
		ResetComposition(window_handle);
	}
}

void IMM32Manager::ResetComposition(HWND window_handle) {
	is_composing_ = false;
}

void IMM32Manager::CompleteComposition(HWND window_handle, HIMC imm_context) {
	if (is_composing_) {
		::ImmNotifyIME(imm_context, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
		ResetComposition(window_handle);
	}
}

bool IMM32Manager::GetString(HIMC imm_context, WPARAM lparam, int type, wstring& result) {
	if (!(lparam & type)) return false;
	LONG string_size = ::ImmGetCompositionString(imm_context, type, NULL, 0);
	if (string_size <= 0 || string_size % sizeof(wchar_t) != 0) return false;
	result.resize(string_size / sizeof(wchar_t));
	::ImmGetCompositionString(imm_context, type, const_cast<wchar_t*>(result.data()), string_size);
	return true;
}

void IMM32Manager::GetResult(HWND window_handle, LPARAM lparam, wstring& result) {
	HIMC imm_context = ::ImmGetContext(window_handle);
	if (imm_context) {
		GetString(imm_context, lparam, GCS_RESULTSTR, result);
		::ImmReleaseContext(window_handle, imm_context);
	}
}

void IMM32Manager::GetComposition(HWND window_handle, LPARAM lparam, wstring& result) {
	HIMC imm_context = ::ImmGetContext(window_handle);
	if (imm_context) {
		if (GetString(imm_context, lparam, GCS_COMPSTR, result)) {
			is_composing_ = true;
		}
		::ImmReleaseContext(window_handle, imm_context);
	}
}

void IMM32Manager::DisableIME(HWND window_handle) {
	CleanupComposition(window_handle);
	::ImmAssociateContextEx(window_handle, NULL, 0);
}

void IMM32Manager::CancelIME(HWND window_handle) {
	if (is_composing_) {
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			::ImmNotifyIME(imm_context, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
			::ImmReleaseContext(window_handle, imm_context);
		}
		ResetComposition(window_handle);
	}
}

void IMM32Manager::EnableIME(HWND window_handle) {
	::ImmAssociateContextEx(window_handle, NULL, IACE_DEFAULT);
}

void IMM32Manager::UpdateCaretRect(HWND window_handle, Rect caret_rect) {
		caret_rect_ = caret_rect;
		// Move the IME windows.
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			MoveImeWindow(window_handle, imm_context);
			::ImmReleaseContext(window_handle, imm_context);
		}
}

void IMM32Manager::SetUseCompositionWindow(bool use_composition_window) {
	use_composition_window_ = use_composition_window;
}

bool IMM32Manager::IsInputLanguageCJK() const {
	LANGID lang = PRIMARYLANGID(input_language_id_);
	return lang == LANG_CHINESE || lang == LANG_JAPANESE ||
		lang == LANG_KOREAN;
}

bool IMM32Manager::IsImm32ImeActive() {
	return ::ImmGetIMEFileName(::GetKeyboardLayout(0), nullptr, 0) > 0;
}

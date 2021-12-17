#include "ime_input.h"

#pragma comment(lib, "imm32.lib")


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


void IMM32Manager::UpdateCaretRect(HWND hwnd,Rect caret_rect) {
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
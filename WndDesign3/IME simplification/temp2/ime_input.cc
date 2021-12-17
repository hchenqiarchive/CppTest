#include "ime_input.h"

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

LRESULT IMM32Manager::SetImeWindowStyle(HWND window_handle, UINT message,WPARAM wparam, LPARAM lparam) {
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
	if (caret_rect_ != caret_rect) {
		caret_rect_ = caret_rect;
		// Move the IME windows.
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			MoveImeWindow(window_handle, imm_context);
			::ImmReleaseContext(window_handle, imm_context);
		}
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
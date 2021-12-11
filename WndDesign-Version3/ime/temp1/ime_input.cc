#include "ime_input.h"

#pragma comment(lib, "imm32.lib")


ImeInput::ImeInput()
	: _ime_status(false),
	_input_language_id(LANG_USER_DEFAULT),
	_is_composing(false),
	_system_caret(false),
	_caret_rect(region_empty) {
}

ImeInput::~ImeInput() {}

bool ImeInput::SetInputLanguage() {
	// Retrieve the current keyboard layout from Windows and determine whether
	// or not the current input context has IMEs.
	// Also save its input language for language-specific operations required
	// while composing a text.
	HKL keyboard_layout = ::GetKeyboardLayout(0);
	_input_language_id = reinterpret_cast<LANGID>(keyboard_layout);
	_ime_status = (::ImmIsIME(keyboard_layout) == TRUE) ? true : false;
	return _ime_status;
}

void ImeInput::CreateImeWindow(HWND window_handle) {
	if (PRIMARYLANGID(_input_language_id) == LANG_CHINESE || PRIMARYLANGID(_input_language_id) == LANG_JAPANESE) {
		if (!_system_caret) {
			if (::CreateCaret(window_handle, NULL, 1, 1)) {
				_system_caret = true;
			}
		}
	}

	// Restore the positions of the IME windows.
	UpdateImeWindow(window_handle);
}

void ImeInput::SetImeWindowStyle(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam, BOOL* handled) {
	*handled = TRUE;
	lparam &= ~ISC_SHOWUICOMPOSITIONWINDOW;
	::DefWindowProc(window_handle, message, wparam, lparam);
}

void ImeInput::DestroyImeWindow(HWND window_handle) {
	// Destroy the system caret if we have created for this IME input context.
	if (_system_caret) {
		::DestroyCaret();
		_system_caret = false;
	}
}

void ImeInput::MoveImeWindow(HWND window_handle, HIMC imm_context) {
	int x = _caret_rect.point.x;
	int y = _caret_rect.point.y;
	const int kCaretMargin = 1;

	CANDIDATEFORM candidate_position = { 0, CFS_CANDIDATEPOS, {x, y},
										{0, 0, 0, 0} };
	::ImmSetCandidateWindow(imm_context, &candidate_position);
	if (_system_caret) {
		switch (PRIMARYLANGID(_input_language_id)) {
		case LANG_JAPANESE:
			::SetCaretPos(x, y + _caret_rect.size.height);
			break;
		default:
			::SetCaretPos(x, y);
			break;
		}
	}

	if (PRIMARYLANGID(_input_language_id) == LANG_KOREAN) {
		y += kCaretMargin;
	}

	CANDIDATEFORM exclude_rectangle = { 0, CFS_EXCLUDE, {x, y},
		{x, y, x + _caret_rect.size.width, y + _caret_rect.size.height} };
	::ImmSetCandidateWindow(imm_context, &exclude_rectangle);

	POINT point = { x, y };
	::ImmSetStatusWindowPos(imm_context, &point);

	COMPOSITIONFORM composition_form = { CFS_POINT, point, {} };
	::ImmSetCompositionWindow(imm_context, &composition_form);
}

void ImeInput::UpdateImeWindow(HWND window_handle) {
	// Just move the IME window attached to the given window.
	if (_caret_rect.point.x >= 0 && _caret_rect.point.y >= 0) {
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			MoveImeWindow(window_handle, imm_context);
			::ImmReleaseContext(window_handle, imm_context);
		}
	}
}

void ImeInput::CleanupComposition(HWND window_handle) {
	// Notify the IMM attached to the given window to complete the ongoing
	// composition, (this case happens when the given window is de-activated
	// while composing a text and re-activated), and reset the omposition status.
	if (_is_composing) {
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			::ImmNotifyIME(imm_context, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
			::ImmReleaseContext(window_handle, imm_context);
		}
		ResetComposition(window_handle);
	}
}

void ImeInput::ResetComposition(HWND window_handle) {
	// Currently, just reset the composition status.
	_is_composing = false;
}

void ImeInput::CompleteComposition(HWND window_handle, HIMC imm_context) {
	// We have to confirm there is an ongoing composition before completing it.
	// This is for preventing some IMEs from getting confused while completing an
	// ongoing composition even if they do not have any ongoing compositions.)
	if (_is_composing) {
		::ImmNotifyIME(imm_context, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
		ResetComposition(window_handle);
	}
}

bool ImeInput::GetString(HIMC imm_context, WPARAM lparam, int type, wstring* composition) {
	bool result = false;
	if (lparam & type) {
		int string_size = ::ImmGetCompositionString(imm_context, type, NULL, 0);
		if (string_size > 0) {
			int string_length = string_size / sizeof(wchar_t);
			composition->resize(string_length);
			// Fill the given ImeComposition object.
			::ImmGetCompositionString(imm_context, type, const_cast<wchar_t*>(composition->data()), string_size);
			result = true;
		}
	}
	return result;
}

bool ImeInput::GetResult(HWND window_handle, LPARAM lparam, wstring* composition) {
	bool result = false;
	HIMC imm_context = ::ImmGetContext(window_handle);
	if (imm_context) {
		// Copy the result string to the ImeComposition object.
		result = GetString(imm_context, lparam, GCS_RESULTSTR, composition);

		// Reset all the other parameters because a result string does not
		// have composition attributes.
		::ImmReleaseContext(window_handle, imm_context);
	}
	return result;
}

bool ImeInput::GetComposition(HWND window_handle, LPARAM lparam, wstring* composition) {
	bool result = false;
	HIMC imm_context = ::ImmGetContext(window_handle);
	if (imm_context) {		
		// Copy the composition string to the ImeComposition object.
		result = GetString(imm_context, lparam, GCS_COMPSTR, composition);

		// Mark that there is an ongoing composition.
		_is_composing = true;
		::ImmReleaseContext(window_handle, imm_context);
	}
	return result;
}

void ImeInput::DisableIME(HWND window_handle) {
	// A renderer process have moved its input focus to a password input
	// when there is an ongoing composition, e.g. a user has clicked a
	// mouse button and selected a password input while composing a text.
	// For this case, we have to complete the ongoing composition and
	// clean up the resources attached to this object BEFORE DISABLING THE IME.
	CleanupComposition(window_handle);
	::ImmAssociateContextEx(window_handle, NULL, 0);
}

void ImeInput::CancelIME(HWND window_handle) {
	if (_is_composing) {
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			::ImmNotifyIME(imm_context, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
			::ImmReleaseContext(window_handle, imm_context);
		}
		ResetComposition(window_handle);
	}
}

void ImeInput::EnableIME(HWND window_handle) {
	::ImmAssociateContextEx(window_handle, NULL, IACE_DEFAULT);
}

void ImeInput::UpdateCaretRect(HWND window_handle, Rect caret_rect) {
	if (_caret_rect != caret_rect) {
		_caret_rect = caret_rect;
		// Move the IME windows.
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			MoveImeWindow(window_handle, imm_context);
			::ImmReleaseContext(window_handle, imm_context);
		}
	}
}
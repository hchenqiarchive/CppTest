#include "win32_ime_input.h"

#pragma comment(lib, "imm32.lib")


ImeInput ime;


void ImeInput::EnableIME(HWND window_handle) {
	::ImmAssociateContextEx(window_handle, NULL, IACE_DEFAULT);
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
		ResetComposition();
	}
}

bool ImeInput::UpdateInputLanguage() {
	// Retrieve the current keyboard layout from Windows and determine whether
	// or not the current input context has IMEs.
	HKL keyboard_layout = ::GetKeyboardLayout(0);
	_language = static_cast<LANGID>(reinterpret_cast<size_t>(keyboard_layout));
	_has_ime = (::ImmIsIME(keyboard_layout) == TRUE) ? true : false;
	return _has_ime;
}


void ImeInput::CreateImeWindow(HWND window_handle) {
	// Create a temporary system caret for Chinese IMEs and use it during this input context.
	if (PRIMARYLANGID(_language) == LANG_CHINESE || PRIMARYLANGID(_language) == LANG_JAPANESE) {
		if (!_has_system_caret) {
			if (::CreateCaret(window_handle, NULL, 1, 1)) {
				_has_system_caret = true;
			}
		}
	}
	// Restore the positions of the IME windows.
	UpdateImeWindow(window_handle);
}

void ImeInput::SetImeWindowStyle(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam) {
	// To prevent the IMM (Input Method Manager) from displaying the IME
	// composition window, Update the styles of the IME windows and EXPLICITLY
	// call ::DefWindowProc() here.
	lparam &= ~ISC_SHOWUICOMPOSITIONWINDOW;
	::DefWindowProc(window_handle, message, wparam, lparam);
}

void ImeInput::DestroyImeWindow(HWND window_handle) {
	// Destroy the system caret if we have created for this IME input context.
	if (_has_system_caret) {
		::DestroyCaret();
		_has_system_caret = false;
	}
}

void ImeInput::MoveImeWindow(HIMC imm_context) {
	int x = _caret_rect.left;
	int y = _caret_rect.top;
	const int kCaretMargin = 1;

	CANDIDATEFORM candidate_position = { 0, CFS_CANDIDATEPOS, {x, y}, {0, 0, 0, 0} };
	::ImmSetCandidateWindow(imm_context, &candidate_position);

	if (_has_system_caret) {
		if (PRIMARYLANGID(_language) == LANG_JAPANESE) {
			::SetCaretPos(x, _caret_rect.bottom);
		} else{
			::SetCaretPos(x, y);
		}
	}

	if (PRIMARYLANGID(_language) == LANG_KOREAN) {
		y += kCaretMargin;
	}

	CANDIDATEFORM exclude_rectangle = { 0, CFS_EXCLUDE, {x, y},
		{x, y, _caret_rect.right, _caret_rect.bottom} };
	::ImmSetCandidateWindow(imm_context, &exclude_rectangle);
}

void ImeInput::UpdateImeWindow(HWND window_handle) {
	// Just move the IME window attached to the given window.
	if (_caret_rect.left >= 0 && _caret_rect.top >= 0) {
		HIMC imm_context = ::ImmGetContext(window_handle);
		if (imm_context) {
			MoveImeWindow(imm_context);
			::ImmReleaseContext(window_handle, imm_context);
		}
	}
}

void ImeInput::UpdateCaretRect(HWND window_handle, RECT caret_rect) {
	// Save the caret position, and Update the position of the IME window.
	// This update is used for moving an IME window when a renderer process
	// resize/moves the input caret.
	_caret_rect = caret_rect;
	// Move the IME windows.
	HIMC imm_context = ::ImmGetContext(window_handle);
	if (imm_context) {
		MoveImeWindow(imm_context);
		::ImmReleaseContext(window_handle, imm_context);
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
		ResetComposition();
	}
}

void ImeInput::ResetComposition() {
	_is_composing = false;	// Currently, just reset the composition status.
	_composition.clear();
	_result.clear();
}

void ImeInput::CompleteComposition(HIMC imm_context) {
	// We have to confirm there is an ongoing composition before completing it.
	// This is for preventing some IMEs from getting confused while completing an
	// ongoing composition even if they do not have any ongoing compositions.)
	if (_is_composing) {
		::ImmNotifyIME(imm_context, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
		ResetComposition();
	}
}

void ImeInput::GetCompositionInfo(HIMC imm_context, LPARAM lparam, wstring& composition) {
	// We only care about GCS_COMPATTR, GCS_COMPCLAUSE and GCS_CURSORPOS, and
	// convert them into underlines and selection range respectively.
	uint length = static_cast<uint>(composition.length());
}

bool ImeInput::GetString(HIMC imm_context, WPARAM lparam, uint type, wstring& composition) {
	bool result = false;
	if (lparam & type) {
		uint string_size = ::ImmGetCompositionString(imm_context, type, NULL, 0);
		if (string_size > 0) {
			uint string_length = string_size / sizeof(wchar_t);
			composition.resize(string_length);
			// Fill the given ImeComposition object.
			::ImmGetCompositionString(imm_context, type, const_cast<wchar_t*>(composition.data()), string_size);
			result = true;
		}
	}
	return result;
}

bool ImeInput::UpdateResult(HWND window_handle, LPARAM lparam) {
	bool result = false;
	HIMC imm_context = ::ImmGetContext(window_handle);
	if (imm_context) {
		// Copy the result string to the ImeComposition object.
		result = GetString(imm_context, lparam, GCS_RESULTSTR, _result);

		// Reset all the other parameters because a result string does not
		// have composition attributes.
		::ImmReleaseContext(window_handle, imm_context);
	}
	return result;
}

bool ImeInput::UpdateComposition(HWND window_handle, LPARAM lparam) {
	bool result = false;
	HIMC imm_context = ::ImmGetContext(window_handle);
	if (imm_context) {
		// Copy the composition string to the ImeComposition object.
		result = GetString(imm_context, lparam, GCS_COMPSTR, _composition);

		// Retrieve the composition underlines and selection range information.
		GetCompositionInfo(imm_context, lparam, _composition);

		// Mark that there is an ongoing composition.
		_is_composing = true;
		::ImmReleaseContext(window_handle, imm_context);
	}
	return result;
}
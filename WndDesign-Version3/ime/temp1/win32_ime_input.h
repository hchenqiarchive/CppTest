#pragma once

#include <Windows.h>

#include <string>

using std::wstring;
using uint = unsigned int;

class ImeInput {

	//////////////////////////////////////////////////////////
	////                    IME Status                    ////
	//////////////////////////////////////////////////////////
private:
	bool _has_ime = false; // True if the current input context has IMEs.
	LANGID _language;  // !!! Deprecated !!!
public:
	// Enables the IME attached to the given window.
	void EnableIME(HWND window_handle);
	// Disables the IME attached to the given window.
	void DisableIME(HWND window_handle);
	// Cancels an ongoing composition of the IME attached to the given window.
	void CancelIME(HWND window_handle);
	// Retrieves the input language from Windows and update it.
	// Returns true if the given input language has IMEs.
	bool UpdateInputLanguage();


	//////////////////////////////////////////////////////////
	////                    IME Window                    ////
	//////////////////////////////////////////////////////////
private:
	bool _has_system_caret = false;	 // True if the current input context has created a system caret 
									 //   to set the position of a IME window.
	RECT _caret_rect = {10,10,20,20}; // The rectangle of the input caret retrieved from a renderer process.

private:
	// Updates the position of the IME windows.
	void MoveImeWindow(HIMC imm_context);
public:
	// Creates the IME windows, and allocate required resources for them.
	void CreateImeWindow(HWND window_handle);
	// Updates the style of the IME windows.
	void SetImeWindowStyle(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);
	// Destroys the IME windows and all the resources attached to them.
	void DestroyImeWindow(HWND window_handle);
	// Updates the caret position of the given window.
	void UpdateCaretRect(HWND window_handle, RECT caret_rect);
	// Updates the position of the IME windows.
	void UpdateImeWindow(HWND window_handle);


	/////////////////////////////////////////////////////////
	////                   Composition                   ////
	/////////////////////////////////////////////////////////
private:
	bool _is_composing = false;  // True if there is an ongoing composition.
	wstring _composition; // The current composition string.
	wstring _result;      // The composition result string.

private:
	// Retrieves the composition information.
	void GetCompositionInfo(HIMC imm_context, LPARAM lparam, wstring& composition);
	// Completes the ongoing composition if it exists.
	void CompleteComposition(HIMC imm_context);
	// Retrieves a string from the IMM.
	bool GetString(HIMC imm_context, WPARAM lparam, uint type, wstring& composition);

public:
	// Retrieves a composition result of the ongoing composition and returns true if it exists.
	bool UpdateResult(HWND window_handle, LPARAM lparam);
	// Retrieves the current composition status of the ongoing composition and returns true if it is updated.
	bool UpdateComposition(HWND window_handle, LPARAM lparam);

	// Cleans up the all resources attached to the given ImeInput object, and reset its composition status.
	void CleanupComposition(HWND window_handle);
	// Resets the composition status.
	void ResetComposition();

public:
	bool IsComposing() const { return _is_composing; }
	const wstring& GetComposition() const { return _composition; }
	const wstring& GetResult() const { return _result; }
};

extern ImeInput ime;
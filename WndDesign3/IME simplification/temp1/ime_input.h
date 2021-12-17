// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IME_INPUT_H_
#define CHROME_BROWSER_IME_INPUT_H_

#include "../../../WndDesign/WndDesign/geometry/geometry.h"

#include <string>
#include <vector>

#include <windows.h>

using WndDesign::Rect;
using WndDesign::uint;
using WndDesign::region_empty;

using std::wstring;

// This class controls the IMM (Input Method Manager) through IMM32 APIs and
// enables it to retrieve the string being controled by the IMM. (I wrote
// a note to describe the reason why I do not use 'IME' but 'IMM' below.)
// NOTE(hbono):
//   Fortunately or unfortunately, TSF (Text Service Framework) and
//   CUAS (Cicero Unaware Application Support) allows IMM32 APIs for
//   retrieving not only the inputs from IMEs (Input Method Editors), used
//   only for inputting East-Asian language texts, but also the ones from
//   tablets (on Windows XP Tablet PC Edition and Windows Vista), voice
//   recognizers (e.g. ViaVoice and Microsoft Office), etc.
//   We can disable TSF and CUAS in Windows XP Tablet PC Edition. On the other
//   hand, we can NEVER disable either TSF or CUAS in Windows Vista, i.e.
//   THIS CLASS IS NOT ONLY USED ON THE INPUT CONTEXTS OF EAST-ASIAN
//   LANGUAGES BUT ALSO USED ON THE INPUT CONTEXTS OF ALL LANGUAGES.
class ImeInput {
public:
	ImeInput();
	~ImeInput();

	// Retrieves whether or not there is an ongoing composition.
	bool is_composing() const { return _is_composing; }

	// Retrieves the input language from Windows and update it.
	// Return values
	//   * true
	//     The given input language has IMEs.
	//   * false
	//     The given input language does not have IMEs.
	bool SetInputLanguage();

	// Creates the IME windows, and allocate required resources for them.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	void CreateImeWindow(HWND window_handle);

	// Updates the style of the IME windows.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	//   * message [in] (UINT)
	//   * wparam [in] (WPARAM)
	//   * lparam [in] (LPARAM)
	//     Represent the windows message of the caller.
	//     These parameters are used for verifying if this function is called
	//     in a handler function for WM_IME_SETCONTEXT messages because this
	//     function uses ::DefWindowProc() to update the style.
	//     A caller just has to pass the input parameters for the handler
	//     function without modifications.
	//   * handled [out] (BOOL*)
	//     Returns ::DefWindowProc() is really called in this function.
	//     PLEASE DO NOT CALL ::DefWindowProc() IF THIS VALUE IS TRUE!
	//     All the window styles set in this function are over-written when
	//     calling ::DefWindowProc() after returning this function.
	void SetImeWindowStyle(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam, BOOL* handled);

	// Destroys the IME windows and all the resources attached to them.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	void DestroyImeWindow(HWND window_handle);

	// Updates the position of the IME windows.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	void UpdateImeWindow(HWND window_handle);

	// Cleans up the all resources attached to the given ImeInput object, and
	// reset its composition status.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	void CleanupComposition(HWND window_handle);

	// Resets the composition status.
	// Cancel the ongoing composition if it exists.
	// NOTE(hbono): This method does not release the allocated resources.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	void ResetComposition(HWND window_handle);

	// Retrieves a composition result of the ongoing composition if it exists.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	//   * lparam [in] (LPARAM)
	//     Specifies the updated members of the ongoing composition, and must be
	//     the same parameter of a WM_IME_COMPOSITION message handler.
	//     This parameter is used for checking if the ongoing composition has
	//     its result string,
	//   * composition [out] (ImeComposition)
	//     Represents the struct contains the composition result.
	// Return values
	//   * true
	//     The ongoing composition has a composition result.
	//   * false
	//     The ongoing composition does not have composition results.
	// Remarks
	//   This function is designed for being called from WM_IME_COMPOSITION
	//   message handlers.
	bool GetResult(HWND window_handle, LPARAM lparam, wstring* composition);

	// Retrieves the current composition status of the ongoing composition.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	//   * lparam [in] (LPARAM)
	//     Specifies the updated members of the ongoing composition, and must be
	//     the same parameter of a WM_IME_COMPOSITION message handler.
	//     This parameter is used for checking if the ongoing composition has
	//     its result string,
	//   * composition [out] (ImeComposition)
	//     Represents the struct contains the composition status.
	// Return values
	//   * true
	//     The status of the ongoing composition is updated.
	//   * false
	//     The status of the ongoing composition is not updated.
	// Remarks
	//   This function is designed for being called from WM_IME_COMPOSITION
	//   message handlers.
	bool GetComposition(HWND window_handle, LPARAM lparam, wstring* composition);

	// Enables the IME attached to the given window, i.e. allows user-input
	// events to be dispatched to the IME.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	//   * complete [in] (bool)
	//     Represents whether or not to complete the ongoing composition.
	//     + true
	//       After finishing the ongoing composition and close its IME windows,
	//       start another composition and display its IME windows to the given
	//       position.
	//     + false
	//       Just move the IME windows of the ongoing composition to the given
	//       position without finishing it.
	void EnableIME(HWND window_handle);

	// Disables the IME attached to the given window, i.e. prohibits any user-input
	// events from being dispatched to the IME.
	// In Chrome, this function is used when:
	//   * a renreder process sets its input focus to a password input.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	void DisableIME(HWND window_handle);

	// Cancels an ongoing composition of the IME attached to the given window.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	void CancelIME(HWND window_handle);

	// Updates the caret position of the given window.
	// Parameters
	//   * window_handle [in] (HWND)
	//     Represents the window handle of the caller.
	//   * caret_rect [in] (const gfx::Rect&)
	//     Represent the rectangle of the input caret.
	//     This rectangle is used for controlling the positions of IME windows.
	void UpdateCaretRect(HWND window_handle, Rect caret_rect);

protected:
	// Updates the position of the IME windows.
	void MoveImeWindow(HWND window_handle, HIMC imm_context);

	// Completes the ongoing composition if it exists.
	void CompleteComposition(HWND window_handle, HIMC imm_context);

	// Retrieves a string from the IMM.
	bool GetString(HIMC imm_context, WPARAM lparam, int type, wstring* composition);

private:
	// Represents whether or not there is an ongoing composition in a browser process.
	bool _is_composing;

	// This value represents whether or not the current input context has IMEs.
	// The following table shows the list of IME status:
	//   true :  The current input language has IMEs.
	//   false : The current input language does not have IMEs.
	bool _ime_status;
	LANGID _input_language_id;

	// Represents whether or not the current input context has created a system
	// caret to set the position of its IME candidate window.
	//   * true: it creates a system caret.
	//   * false: it does not create a system caret.
	bool _system_caret;

	// The rectangle of the input caret retrieved from a renderer process.
	Rect _caret_rect;
};


#endif  // CHROME_BROWSER_IME_INPUT_H_
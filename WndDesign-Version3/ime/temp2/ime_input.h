
#include "../../../WndDesign/WndDesign/geometry/geometry.h"

#include <string>

#include <Windows.h>

using WndDesign::Rect;
using WndDesign::uint;
using WndDesign::region_empty;

using std::wstring;


class IMM32Manager {
public:
	bool is_composing() const { return is_composing_; }

	IMM32Manager()
		: is_composing_(false),
		input_language_id_(LANG_USER_DEFAULT),
		system_caret_(false),
		caret_rect_(-1, -1, 0, 0),
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
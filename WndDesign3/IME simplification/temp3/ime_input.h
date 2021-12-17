
#include "../../../WndDesign/WndDesign/geometry/geometry.h"

#include <string>

#include <Windows.h>

using WndDesign::Rect;
using WndDesign::uint;
using WndDesign::region_empty;

using std::wstring;


class IMM32Manager {
public:
	IMM32Manager() : is_composing(false) {}
	~IMM32Manager() {}

	bool is_composing;

	void CleanupComposition(HWND hwnd);

	void EnableIME(HWND hwnd);
	void DisableIME(HWND hwnd);
	void CancelIME(HWND hwnd);

	void UpdateCaretRect(HWND hwnd, Rect caret_rect);

	void GetString(HIMC imm_context, WPARAM lparam, int type, wstring& result);

	void GetResult(HWND hwnd, LPARAM lparam, wstring& result);
	void GetComposition(HWND hwnd, LPARAM lparam, wstring& result);
};
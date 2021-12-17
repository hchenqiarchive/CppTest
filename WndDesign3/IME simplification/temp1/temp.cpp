
  
    switch (msg) {
    case WM_INPUTLANGCHANGE:

        break;
    case WM_IME_SETCONTEXT: {
        HIMC imm_context = ::ImmGetContext(hwnd);
        COMPOSITIONFORM cf = { CFS_POINT, {0, 0} };
        ::ImmSetCompositionWindow(imm_context, &cf);
        ::ImmReleaseContext(hwnd, imm_context);
        break;
    }
    case WM_IME_STARTCOMPOSITION:
        break;
    case WM_IME_COMPOSITION:
        if (lparam & GCS_RESULTSTR) {
            HIMC himc = ImmGetContext(hwnd);
            int size = ImmGetCompositionString(himc, GCS_RESULTSTR, NULL, 0);
            std::wstring str; str.resize(size);
            ImmGetCompositionString(himc, GCS_RESULTSTR, str.data(), size);
            ImmReleaseContext(hwnd, himc);
        }
        break;
    case WM_IME_ENDCOMPOSITION:

		break;

    case WM_DESTROY: PostQuitMessage(0); return 0;
    default:	return DefWindowProc(hwnd, msg, wparam, lparam);

	}
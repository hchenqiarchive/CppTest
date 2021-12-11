#include <ShlObj.h>

int main() {
    OleInitialize(nullptr);

    wchar_t buffer[MAX_PATH] = {};

    BROWSEINFOW bi = {};
    bi.hwndOwner = nullptr;
    bi.pszDisplayName = buffer;
    bi.lpszTitle = L"";
    bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_BROWSEFORCOMPUTER | BIF_UAHINT | BIF_USENEWUI;
    ITEMIDLIST* pidl = SHBrowseForFolderW(&bi);
    SHGetPathFromIDListW(pidl, buffer);
}
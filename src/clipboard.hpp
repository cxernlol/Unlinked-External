#pragma once

#include <Windows.h>
#include <string>

namespace clipboard {

inline bool Copy(const std::string& text) {
    if (!OpenClipboard(nullptr)) return false;
    EmptyClipboard();
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hGlobal) {
        CloseClipboard();
        return false;
    }
    memcpy(GlobalLock(hGlobal), text.c_str(), text.size() + 1);
    GlobalUnlock(hGlobal);
    SetClipboardData(CF_TEXT, hGlobal);
    CloseClipboard();
    return true;
}

inline std::string Paste() {
    if (!OpenClipboard(nullptr)) return "";
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (!hData) {
        CloseClipboard();
        return "";
    }
    char* text = static_cast<char*>(GlobalLock(hData));
    std::string result(text ? text : "");
    GlobalUnlock(hData);
    CloseClipboard();
    return result;
}

}

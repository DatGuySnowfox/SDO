#pragma once
// Minimal, dependency-free file logger, independent of UE4SS's own
// Output::send/console pipeline. Exists solely to answer "did this code path
// actually run" when Output::send's own destination is in question — not a
// replacement for normal logging.
#include <cstdio>
#include <string>
#include <windows.h>

inline void debug_log(const std::string& line)
{
    wchar_t path[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"APPDATA", path, MAX_PATH);
    std::wstring dir = (n > 0 && n < MAX_PATH) ? std::wstring(path, n) : L"C:\\Temp";
    dir += L"\\SurrounDeadBridge";
    CreateDirectoryW(dir.c_str(), nullptr);
    std::wstring file = dir + L"\\debug.log";

    FILE* f = nullptr;
    if (_wfopen_s(&f, file.c_str(), L"a") == 0 && f) {
        fprintf(f, "%s\n", line.c_str());
        fclose(f);
    }
}

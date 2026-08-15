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

    // Local HH:MM:SS.mmm on every line — added 2026-08-13. Without this,
    // correlating an event here against SDB.log or the server's own log
    // (also now timestamped, see index.js) meant matching by line-number
    // proximity and guessing, across three separate files with no shared
    // clock reference at all.
    SYSTEMTIME st;
    GetLocalTime(&st);
    char ts[16];
    snprintf(ts, sizeof(ts), "%02u:%02u:%02u.%03u", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    FILE* f = nullptr;
    if (_wfopen_s(&f, file.c_str(), L"a") == 0 && f) {
        fprintf(f, "[%s] %s\n", ts, line.c_str());
        fclose(f);
    }
}

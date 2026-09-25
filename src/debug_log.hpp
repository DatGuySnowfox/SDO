#pragma once
// Minimal, dependency-free file logger, independent of UE4SS's own
// Output::send/console pipeline. Exists solely to answer "did this code path
// actually run" when Output::send's own destination is in question - not a
// replacement for normal logging.
//
// Location (2026-09-24): defaults to %APPDATA%\SDO, but
// SDO_LOG_DIR overrides it, so a dev checkout can collect logs next to the
// source instead of three directories away. Read straight from the
// environment rather than session.cfg: debug_log() runs before any config
// parsing and must not depend on init order.
//
// Note the other two logs are UE4SS-owned and land in ITS root directory
// (Binaries/Win64/ue4ss as of the post-3.0.1 layout): SDO.log for
// Output::send output, UE4SS.log for UE4SS core. Only this file is ours.
#include <cstdio>
#include <string>
#include <windows.h>

// Rotate past this size. The previous unbounded file reached 377 MB, which is
// slow to grep and was silently the largest thing in %APPDATA%.
inline constexpr long long kDebugLogMaxBytes = 64LL * 1024 * 1024;

inline std::wstring debug_log_dir()
{
    wchar_t buf[MAX_PATH];
    DWORD n = GetEnvironmentVariableW(L"SDO_LOG_DIR", buf, MAX_PATH);
    if (n > 0 && n < MAX_PATH) return std::wstring(buf, n);

    n = GetEnvironmentVariableW(L"APPDATA", buf, MAX_PATH);
    std::wstring dir = (n > 0 && n < MAX_PATH) ? std::wstring(buf, n) : L"C:\\Temp";
    return dir + L"\\SDO";
}

// Renames an oversized log to .prev (replacing any existing .prev) so a
// session always starts with room, without losing the immediately-previous
// run. Checked on first use and then only occasionally - this is called from
// per-tick paths and must not stat the file on every line.
inline void debug_log_rotate_if_needed(const std::wstring& file, bool force)
{
    static int s_counter = 0;
    if (!force && (++s_counter % 4096) != 0) return;

    WIN32_FILE_ATTRIBUTE_DATA fad{};
    if (!GetFileAttributesExW(file.c_str(), GetFileExInfoStandard, &fad)) return;
    const long long size = (static_cast<long long>(fad.nFileSizeHigh) << 32) | fad.nFileSizeLow;
    if (size < kDebugLogMaxBytes) return;

    const std::wstring prev = file + L".prev";
    DeleteFileW(prev.c_str());
    MoveFileW(file.c_str(), prev.c_str());
}

inline void debug_log(const std::string& line)
{
    static const std::wstring s_dir  = [] { auto d = debug_log_dir(); CreateDirectoryW(d.c_str(), nullptr); return d; }();
    static const std::wstring s_file = s_dir + L"\\debug.log";
    static const bool s_first = [] { debug_log_rotate_if_needed(s_dir + L"\\debug.log", true); return true; }();
    (void)s_first;

    debug_log_rotate_if_needed(s_file, false);

    // Local HH:MM:SS.mmm on every line - added 2026-08-13. Without this,
    // correlating an event here against SDO.log or the server's own log
    // (also now timestamped, see index.js) meant matching by line-number
    // proximity and guessing, across three separate files with no shared
    // clock reference at all.
    SYSTEMTIME st;
    GetLocalTime(&st);
    char ts[16];
    snprintf(ts, sizeof(ts), "%02u:%02u:%02u.%03u", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    FILE* f = nullptr;
    if (_wfopen_s(&f, s_file.c_str(), L"a") == 0 && f) {
        fprintf(f, "[%s] %s\n", ts, line.c_str());
        fclose(f);
    }
}

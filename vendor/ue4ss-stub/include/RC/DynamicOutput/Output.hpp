#pragma once
// Minimal Output shim.
// Writes to a dedicated SDB.log file so messages are visible regardless of
// whether a debug console or UE4SS output device is registered.

#include <string>
#include <format>
#include <cstdio>
#include <Windows.h>
#include "../Unreal/Core.hpp"

namespace RC::Output {

namespace detail {
inline FILE* log_file()
{
    static FILE* f = nullptr;
    if (!f) {
        // Try next to the game exe first, fall back to C:\temp\SDB.log.
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(nullptr, path, MAX_PATH);
        wchar_t* last_sep = wcsrchr(path, L'\\');
        if (last_sep) wcscpy_s(last_sep + 1, MAX_PATH - (last_sep - path + 1), L"SDB.log");
        f = _wfopen(path, L"a");
        if (!f) f = _wfopen(L"C:\\temp\\SDB.log", L"a");
    }
    return f;
}
} // namespace detail

template<LogLevel level = LogLevel::Normal, typename... Args>
inline void send(const wchar_t* fmt, Args&&... args)
{
    const std::wstring msg = std::vformat(fmt, std::make_wformat_args(args...));
    OutputDebugStringW(msg.c_str());
    if (FILE* f = detail::log_file()) {
        std::fputws(msg.c_str(), f);
        std::fflush(f);
    }
}

} // namespace RC::Output

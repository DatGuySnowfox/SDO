#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "../Unreal/Core.hpp"

namespace RC {

// StringType mirrors deps/first/String/include/String/StringType.hpp.
// Windows builds use wchar_t (FORCE_U16 is Linux-only), so this must stay
// wchar_t to keep every StringType member the same size as UE4SS's own.
using CharType       = wchar_t;
using StringType     = std::basic_string<CharType>;
using StringViewType = std::basic_string_view<CharType>;

// Forward declarations only. GUITab appears solely inside
// std::shared_ptr<GUITab> within a std::vector, and both of those have a
// layout independent of the pointee, so the full definition (which would drag
// in UE4SS GUI and imgui) is not needed to be ABI-correct.
namespace GUI { class GUITab; }
namespace LuaMadeSimple { class Lua; }

// MIRRORS RC::CppUserModBase from CURRENT UE4SS (post-3.0.1 layout), checked
// against UE4SS/include/Mod/CppUserModBase.hpp on the main branch 2026-09-24.
//
// This is an ABI replica, not a re-implementation: UE4SS allocates nothing
// here, but it constructs the base via the imported constructor, calls our
// overrides through its own vtable, and destroys it through the imported
// destructor. So BOTH the data member order and the virtual declaration order
// must match the real class exactly.
//
// The previous version of this file mirrored v3.0.1 and was silently wrong
// against a newer UE4SS: its first member was a std::wstring where the real
// class now begins with a std::vector, so simply assigning ModName in a
// derived constructor wrote a string over a vector's internals and corrupted
// the heap before the mod could log anything. When UE4SS is updated, re-check
// this file first — a mismatch here fails long before any of the mod's own
// logic runs.
//
// UE4SS and the mod must also be built with the same C runtime and
// configuration, since std::string/std::vector cross the boundary by value.
class CppUserModBase {
protected:
    // Must remain the FIRST member.
    std::vector<std::shared_ptr<GUI::GUITab>> GUITabs{};

public:
    StringType ModName{};
    StringType ModVersion{};
    StringType ModDescription{};
    StringType ModAuthors{};
    StringType ModIntendedSDKVersion{};

public:
    RC_API CppUserModBase();
    RC_API virtual ~CppUserModBase();

    // --- vtable order below must match the real class exactly ---

    virtual void on_update() {}
    virtual void on_unreal_init() {}
    virtual void on_ui_init() {}
    virtual void on_program_start() {}

    // Deprecated Lua overloads. Unused here, but they still occupy vtable
    // slots, so removing them would shift everything after them.
    virtual void on_lua_start(StringViewType, LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                              LuaMadeSimple::Lua&, std::vector<LuaMadeSimple::Lua*>&) {}
    virtual void on_lua_start(LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                              LuaMadeSimple::Lua&, std::vector<LuaMadeSimple::Lua*>&) {}
    virtual void on_lua_stop(StringViewType, LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                             LuaMadeSimple::Lua&, std::vector<LuaMadeSimple::Lua*>&) {}
    virtual void on_lua_stop(LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                             LuaMadeSimple::Lua&, std::vector<LuaMadeSimple::Lua*>&) {}

    virtual void on_dll_load(StringViewType) {}
    virtual void render_tab() {}

    // Current Lua overloads (single hook_lua pointer).
    virtual void on_lua_start(StringViewType, LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                              LuaMadeSimple::Lua&, LuaMadeSimple::Lua*) {}
    virtual void on_lua_start(LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                              LuaMadeSimple::Lua&, LuaMadeSimple::Lua*) {}
    virtual void on_lua_stop(StringViewType, LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                             LuaMadeSimple::Lua&, LuaMadeSimple::Lua*) {}
    virtual void on_lua_stop(LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                             LuaMadeSimple::Lua&, LuaMadeSimple::Lua*) {}

    virtual void on_cpp_mods_loaded() {}
};

} // namespace RC

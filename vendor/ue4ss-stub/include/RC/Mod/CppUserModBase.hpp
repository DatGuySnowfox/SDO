#pragma once
#include <string>
#include <string_view>
#include <vector>
#include "../Unreal/Core.hpp"

namespace RC {

// Opaque proxy – we never actually use Lua from this mod.
namespace LuaMadeSimple { class Lua {}; }

// Mirrors RC::CppUserModBase from UE4SS 3.0.1.
// Virtual method ORDER must match the real class vtable exactly.
// Data members must be declared identically so the imported constructor
// initialises them at the correct offsets.
class CppUserModBase {
public:
    std::wstring ModName{};
    std::wstring ModShortName{};
    std::wstring ModAuthor{};
    std::wstring ModDescription{};

    struct ModVersion { int major{1}, minor{0}, patch{0}; };
    ModVersion ModVersion{};

    bool m_is_installable{true};
    bool m_is_installed{false};

    // Safety buffer: absorbs any private fields present in the real class that
    // we are not aware of, preventing the imported constructor from writing
    // beyond our allocation.
    char _reserved[128]{};

    RC_API CppUserModBase();
    virtual RC_API ~CppUserModBase();

    // vtable order must match UE4SS 3.0.1 CppUserModBase declaration order:
    virtual void on_program_start() {}
    virtual void on_unreal_init()   {}
    virtual void on_update()        {}
    virtual void on_dll_load(std::wstring_view) {}
    virtual void on_uninstall() {}
    virtual void on_lua_start(LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                               LuaMadeSimple::Lua*, std::vector<LuaMadeSimple::Lua*>&) {}
    virtual void on_lua_start(std::wstring_view, LuaMadeSimple::Lua&,
                               LuaMadeSimple::Lua&, LuaMadeSimple::Lua*,
                               std::vector<LuaMadeSimple::Lua*>&) {}
    virtual void on_lua_stop(LuaMadeSimple::Lua&, LuaMadeSimple::Lua&,
                              LuaMadeSimple::Lua*, std::vector<LuaMadeSimple::Lua*>&) {}
    virtual void on_lua_stop(std::wstring_view, LuaMadeSimple::Lua&,
                              LuaMadeSimple::Lua&, LuaMadeSimple::Lua*,
                              std::vector<LuaMadeSimple::Lua*>&) {}
    virtual void render_tab() {}
};

} // namespace RC

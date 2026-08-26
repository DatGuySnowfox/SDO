-- SurrounDeadBridge – xmake build (alternative to CMake)
-- Usage: xmake build
-- Requires UE4SS_SDK env var or set ue4ss_sdk below.

set_project("SurrounDeadBridge")
set_version("1.0.0")
set_xmakever("2.8.5")

add_rules("mode.debug", "mode.release")
set_defaultmode("release")

local ue4ss_sdk = path.join(os.scriptdir(), "vendor/ue4ss-stub")

target("SurrounDeadBridge")
    set_kind("shared")
    set_languages("cxx20")
    set_arch("x64")
    set_symbols("debug")

    add_files("src/protocol.cpp", "src/tcp_client.cpp", "src/proxy_manager.cpp",
              "src/entity_manager.cpp", "src/mod.cpp")
    add_headerfiles("src/*.hpp")

    add_includedirs("src")
    add_includedirs(path.join(ue4ss_sdk, "include"))

    add_linkdirs(path.join(ue4ss_sdk, "lib"))
    add_links("UE4SS", "Ws2_32", "User32")

    add_defines("UNICODE", "_UNICODE", "WIN32_LEAN_AND_MEAN", "NOMINMAX",
                "_CRT_SECURE_NO_WARNINGS")

    if is_plat("windows") then
        add_cxxflags("/W4", "/permissive-", {force = true})
        -- Match UE4SS DLL runtime
        add_cxxflags("/MD", {force = true})
    end

    -- UE4SS C++ mod output convention
    set_filename("main.dll")
    set_targetdir("$(buildir)/out")

-- Standalone wire-protocol round-trip test — deliberately its own target
-- rather than a mode of the mod DLL: protocol.cpp has zero UE4SS/UE5
-- dependency (only stdlib + <Windows.h> for now_micros()), so this builds
-- and runs as a plain console binary with no game, no UE4SS, and no
-- vendor/ue4ss-stub include/link paths involved at all.
-- Usage: xmake build protocol_test && xmake run protocol_test
target("protocol_test")
    set_kind("binary")
    set_languages("cxx20")
    set_arch("x64")
    set_symbols("debug")

    add_files("src/protocol.cpp", "tests/protocol_roundtrip.cpp")
    add_includedirs("src")

    add_defines("UNICODE", "_UNICODE", "WIN32_LEAN_AND_MEAN", "NOMINMAX",
                "_CRT_SECURE_NO_WARNINGS")

    if is_plat("windows") then
        add_cxxflags("/W4", "/permissive-", {force = true})
    end

    set_targetdir("$(buildir)/test_out")

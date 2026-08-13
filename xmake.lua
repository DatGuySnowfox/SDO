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
    add_links("UE4SS", "Ws2_32")

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

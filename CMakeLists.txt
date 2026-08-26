cmake_minimum_required(VERSION 3.21)
project(SurrounDeadBridge VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── UE4SS stub SDK (vendored, generated from UE4SS.dll export table) ──────────
# Run scripts/gen_import_lib.ps1 once in a Visual Studio Developer Prompt to
# produce vendor/ue4ss-stub/lib/UE4SS.lib from vendor/ue4ss-stub/UE4SS.def.
set(UE4SS_STUB "${CMAKE_SOURCE_DIR}/vendor/ue4ss-stub")

# ── Target ───────────────────────────────────────────────────────────────────
add_library(SurrounDeadBridge SHARED
    src/protocol.cpp
    src/tcp_client.cpp
    src/entity_manager.cpp
    src/proxy_manager.cpp
    src/mod.cpp
)

target_include_directories(SurrounDeadBridge PRIVATE
    src/
    "${UE4SS_STUB}/include"
)

find_library(UE4SS_LIB
    NAMES UE4SS
    PATHS "${UE4SS_STUB}/lib"
    REQUIRED
)

target_link_libraries(SurrounDeadBridge PRIVATE
    ${UE4SS_LIB}
    Ws2_32
    User32
    Shell32
)

target_compile_definitions(SurrounDeadBridge PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

if(MSVC)
    target_compile_options(SurrounDeadBridge PRIVATE /W4 /permissive-)
    set_property(TARGET SurrounDeadBridge PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

# Output as main.dll (UE4SS C++ mod convention)
set_target_properties(SurrounDeadBridge PROPERTIES
    OUTPUT_NAME "main"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out"
)

# Copy UE4SS.dll next to main.dll so Windows finds it at runtime.
# Adjust the source path to point to the UE4SS.dll from SD-Online.
if(DEFINED SDB_UE4SS_DLL)
    add_custom_command(TARGET SurrounDeadBridge POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDB_UE4SS_DLL}"
            "$<TARGET_FILE_DIR:SurrounDeadBridge>/UE4SS.dll"
        COMMENT "Copying UE4SS.dll alongside main.dll"
    )
endif()

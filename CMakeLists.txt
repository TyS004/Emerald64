cmake_minimum_required(VERSION 3.15)
project(GCGameEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(./vendor/SDL)

# ── Engine (DLL) ──────────────────────────────
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS engine/src/*.cpp)
add_library(GCGameEngine SHARED ${ENGINE_SOURCES})
target_include_directories(GCGameEngine PUBLIC ./engine/include)
target_link_libraries(GCGameEngine PUBLIC SDL3::SDL3)
target_precompile_headers(GCGameEngine PRIVATE engine/src/pch.h)

# ── Editor (EXE) ──────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS editor/src/*.cpp)
add_executable(GCEditor ${EDITOR_SOURCES})
target_link_libraries(GCEditor PRIVATE GCGameEngine)
target_precompile_headers(GCEditor REUSE_FROM GCGameEngine)
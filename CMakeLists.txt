cmake_minimum_required(VERSION 3.15)
project(GCGameEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(FBX_SDK "/Applications/Autodesk/FBX SDK/2020.3.9")

add_subdirectory(./vendor/SDL)
add_subdirectory(./vendor/glm)
add_subdirectory(./vendor/imgui)

# ── Engine (DLL) ──────────────────────────────
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS engine/src/*.cpp)
add_library(GCGameEngine SHARED ${ENGINE_SOURCES})
target_include_directories(GCGameEngine PUBLIC ./engine/include)
include_directories(${FBX_SDK}/include)
link_directories(${FBX_SDK}/lib/clang/release)
target_link_libraries(GCGameEngine PUBLIC SDL3::SDL3 glm::glm  ImGui::ImGui ${FBX_SDK}/lib/clang/release/libfbxsdk.dylib)
target_precompile_headers(GCGameEngine PRIVATE engine/src/pch.h)

# ── Editor (EXE) ──────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS editor/src/*.cpp)
target_include_directories(GCGameEngine PUBLIC ./editor/include)
add_executable(GCEditor ${EDITOR_SOURCES})
target_link_libraries(GCEditor PRIVATE GCGameEngine)
target_precompile_headers(GCEditor REUSE_FROM GCGameEngine)

# ── Sandbox (EXE) ──────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS sandbox/*.cpp)
add_executable(GCSandbox ${EDITOR_SOURCES})
target_link_libraries(GCSandbox PRIVATE GCGameEngine)
target_precompile_headers(GCSandbox REUSE_FROM GCGameEngine)

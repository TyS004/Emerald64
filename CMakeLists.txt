cmake_minimum_required(VERSION 3.15)
project(E64)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(FBX_SDK "/Applications/Autodesk/FBX SDK/2020.3.9")
set(IMGUI_BACKEND_SDL3  ON CACHE BOOL "" FORCE)
set(IMGUI_BACKEND_SDLGPU3 ON CACHE BOOL "" FORCE)
set(IMGUI_SDL3_INCLUDE "${CMAKE_SOURCE_DIR}/vendor/SDL/include" CACHE PATH "" FORCE)

add_subdirectory(./vendor/SDL)
add_subdirectory(./vendor/glm)
add_subdirectory(./vendor/imgui)

# ── Engine (DLL) ──────────────────────────────
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS engine/src/*.cpp)
add_library(E64 SHARED ${ENGINE_SOURCES})
target_include_directories(E64 PUBLIC ./engine/include)
include_directories(${FBX_SDK}/include)
link_directories(${FBX_SDK}/lib/clang/release)
target_link_libraries(E64 PUBLIC SDL3::SDL3 glm::glm  ImGui::ImGui ${FBX_SDK}/lib/clang/release/libfbxsdk.dylib)
target_precompile_headers(E64 PRIVATE engine/src/pch.h)

# ── Editor (EXE) ──────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS editor/src/*.cpp)
target_include_directories(E64 PUBLIC ./editor/include)
add_executable(E64Editor ${EDITOR_SOURCES})
target_link_libraries(E64Editor PRIVATE E64)
target_precompile_headers(E64Editor REUSE_FROM E64)

# ── Sandbox (EXE) ──────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS sandbox/*.cpp)
add_executable(E64Sandbox ${EDITOR_SOURCES})
target_link_libraries(E64Sandbox PRIVATE E64)
target_precompile_headers(E64Sandbox REUSE_FROM E64)


add_custom_command(TARGET E64 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FBX_SDK}/lib/clang/release/libfbxsdk.dylib"
        "$<TARGET_FILE_DIR:E64>"
)
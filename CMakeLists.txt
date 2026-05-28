cmake_minimum_required(VERSION 3.15)
project(E64)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")

set(IMGUI_BACKEND_SDL3    ON CACHE BOOL "" FORCE)
set(IMGUI_BACKEND_SDLGPU3 ON CACHE BOOL "" FORCE)
set(IMGUI_SDL3_INCLUDE "${CMAKE_SOURCE_DIR}/vendor/SDL/include" CACHE PATH "" FORCE)

set(ASSIMP_BUILD_TESTS                      OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL                          OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT   OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER               ON  CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER               ON  CACHE BOOL "" FORCE)

add_subdirectory(vendor/assimp)
add_subdirectory(vendor/SDL)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/imgui)

# ── Engine (DLL) ──────────────────────────────
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS engine/src/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS engine/include/*.h)

add_library(E64 SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(E64
    PUBLIC
        engine/include
        vendor/glm
    PRIVATE
        ./vendor/json
        ./vendor/stb_img
)

include_directories(E64 PRIVATE ./vendor/stb_img)
include_directories(E64 PRIVATE ./vendor/json)

target_link_libraries(E64 PUBLIC SDL3::SDL3 ImGui::ImGui)
target_precompile_headers(E64 PRIVATE engine/src/pch.h)

# ── Editor (EXE) ──────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS editor/src/*.cpp)
file(GLOB_RECURSE EDITOR_HEADERS CONFIGURE_DEPENDS editor/include/*.h)

add_executable(E64Editor ${EDITOR_SOURCES} ${EDITOR_HEADERS})

target_compile_definitions(E64Editor PRIVATE E64_EDITOR)
target_compile_definitions(E64       PRIVATE E64_EDITOR)

target_include_directories(E64Editor
    PUBLIC
        editor/include
    PRIVATE
        vendor/assimp/include
)

target_link_libraries(E64Editor PRIVATE assimp::assimp E64)
target_precompile_headers(E64Editor REUSE_FROM E64)

# ── Runtime (EXE) ──────────────────────────────
file(GLOB_RECURSE RUNTIME_SOURCES CONFIGURE_DEPENDS n64/*.cpp)

add_executable(E64Runtime ${RUNTIME_SOURCES})

target_link_libraries(E64Runtime PRIVATE E64)
target_precompile_headers(E64Runtime REUSE_FROM E64)

# ── Platform definitions ──────────────────────
if(APPLE)
    target_compile_definitions(E64 PRIVATE E64_APPLE)
elseif(WIN32)
    target_compile_definitions(E64 PRIVATE E64_WIN32)
endif()
cmake_minimum_required(VERSION 3.15)
project(E64)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(IMGUI_BACKEND_SDL3  ON CACHE BOOL "" FORCE)
set(IMGUI_BACKEND_SDLGPU3 ON CACHE BOOL "" FORCE)
set(IMGUI_SDL3_INCLUDE "${CMAKE_SOURCE_DIR}/vendor/SDL/include" CACHE PATH "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Disable warnings-as-errors for Assimp" FORCE)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/assimp)
add_subdirectory(./vendor/SDL)
add_subdirectory(./vendor/glm)
add_subdirectory(./vendor/imgui)

# ── Engine (DLL) ──────────────────────────────
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS engine/src/*.cpp)

add_library(E64 SHARED ${ENGINE_SOURCES})

target_include_directories(E64 PUBLIC ./engine/include)
include_directories(E64 PRIVATE ./vendor/json)
include_directories(E64 PRIVATE ./vendor/stb_img)

target_link_libraries(E64 PUBLIC SDL3::SDL3 glm::glm  ImGui::ImGui)
target_compile_definitions(E64 PRIVATE E64_BUILD_DLL)

# ── Editor (EXE) ──────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS editor/src/*.cpp platform/SDL/src/*.cpp)

add_executable(E64Editor ${EDITOR_SOURCES})

target_include_directories(E64Editor PUBLIC ./editor/include ./platform/SDL/include)
target_include_directories(E64Editor PRIVATE vendor/assimp/include)

target_link_libraries(E64 PRIVATE assimp)
target_link_libraries(E64Editor PRIVATE assimp)
target_link_libraries(E64Editor PRIVATE E64)

# ── Runtime (EXE) ──────────────────────────────
file(GLOB_RECURSE RUNTIME_SOURCES CONFIGURE_DEPENDS runtime/SDL/*.cpp platform/SDL/src/*.cpp)

add_executable(E64Runtime ${RUNTIME_SOURCES})

target_include_directories(E64Runtime PUBLIC ./platform/SDL/include ./runtime/SDL/include)

target_link_libraries(E64Runtime PRIVATE E64)

if(APPLE)
    target_compile_definitions(E64 PUBLIC E64_APPLE)
elseif(WIN32)
    target_compile_definitions(E64 PUBLIC E64_WIN32)

    find_program(DXC_EXECUTABLE dxc REQUIRED)

    file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/assets/shaders/*.hlsl")

    foreach(SHADER ${SHADER_FILES})
        get_filename_component(SHADER_NAME ${SHADER} NAME_WE)

        if(SHADER_NAME MATCHES "_frag$")
            set(PROFILE "ps_6_0")
            set(ENTRY "main")
        elseif(SHADER_NAME MATCHES "_vert$")
            set(PROFILE "vs_6_0")
            set(ENTRY "main")
        else()
            message(WARNING "Skipping ${SHADER_NAME}: no _frag or _vert suffix")
            continue()
        endif()

        set(OUTPUT_DXIL "${CMAKE_SOURCE_DIR}/assets/shaders/${SHADER_NAME}.dxil")

        add_custom_command(
            OUTPUT ${OUTPUT_DXIL}
            COMMAND ${DXC_EXECUTABLE} -T ${PROFILE} -E ${ENTRY} -Fo ${OUTPUT_DXIL} ${SHADER}
            DEPENDS ${SHADER}
            VERBATIM
        )
        list(APPEND SHADER_OUTPUTS ${OUTPUT_DXIL})
    endforeach()

    add_custom_target(Shaders ALL DEPENDS ${SHADER_OUTPUTS})
endif()

set_target_properties(E64 PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
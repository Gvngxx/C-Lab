cmake_minimum_required(VERSION 3.10)
project(LabProg VERSION 1.0 LANGUAGES CXX C)

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(LVNG_BUILD_SHARED "Build shared library" OFF)
option(LVNG_BUILD_EXAMPLES "Build example applications" ON)
option(LVNG_ENABLE_DEBUG "Enable debug features" ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Build third_party libraries
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/assimp)

set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/openal)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# --- Fuentes del motor / aplicación ---
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")

# --- Fuentes de bibliotecas third_party ---
file(GLOB THIRD_PARTY_SOURCES
    "third_party/glad/src/gl.c"
    "third_party/imgui/imgui.cpp"
    "third_party/imgui/imgui_draw.cpp"
    "third_party/imgui/imgui_demo.cpp"
    "third_party/imgui/imgui_widgets.cpp"
    "third_party/imgui/imgui_tables.cpp"
    "third_party/imgui/backends/imgui_impl_glfw.cpp"
    "third_party/imgui/backends/imgui_impl_opengl3.cpp"
)

set(SOURCES
    ${ENGINE_SOURCES}
    ${THIRD_PARTY_SOURCES}
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    third_party/glad/include
    third_party/glfw/include
    third_party/glm
    third_party/imgui
    third_party/imgui/misc
    third_party/imgui/backends
    third_party/json
    third_party/stb
    third_party/assimp/include
    third_party/openal/include
)

if(WIN32 AND MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        opengl32 gdi32 user32 kernel32
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        assimp::assimp
        OpenAL::OpenAL
        OpenGL::GL
        Threads::Threads
        dl
    )

    if(UNIX AND NOT APPLE)
        target_link_libraries(${PROJECT_NAME} PRIVATE X11)
    endif()
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

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

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
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
    ${GLFW_INCLUDE_DIRS}
    third_party/glm
    third_party/imgui
    third_party/imgui/misc
    third_party/imgui/backends
    third_party/json
    third_party/stb
    third_party/tinygltf
    third_party/tinyobj
)

if(WIN32 AND MINGW)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        opengl32 gdi32 user32 kernel32
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${GLFW_LIBRARIES}
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

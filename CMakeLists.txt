cmake_minimum_required(VERSION 3.0)
project(katana_kleaver C)
set(CMAKE_C_STANDARD 99)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/sources/*.c") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include/") # Define PROJECT_INCLUDE to be the path to the include directory of the project
# In CLion /CMake Settings/CMake Options, add: -DCMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE=TRUE, then rebuild

# Check build type and set executable
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
    target_compile_definitions(${PROJECT_NAME} PRIVATE PROFILE_DEBUG)

    # Debug-specific compiler flags
    target_compile_options(${PROJECT_NAME} PRIVATE -g -fno-omit-frame-pointer)
else ()
    if (WIN32)
        add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES} win_res/resources.rc)
    else ()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif ()
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/") # Set the asset path macro to a relative path in release mode
    target_compile_definitions(${PROJECT_NAME} PRIVATE PROFILE_RELEASE)
endif ()

if (EMSCRIPTEN)
    # Default Shell
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 --preload-file assets")
    # Custom Shell
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 --preload-file assets --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/custom_shells/shell_release.html")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PROFILE_RELEASE)
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

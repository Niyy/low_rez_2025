cmake_minimum_required(VERSION 3.16)
project(low_rez)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build tools
#set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")
#set(CMAKE_CXX_COMPILER "C:/ProgramData/mingw64/mingw64/bin/g++.exe")
#set(CMAKE_MAKE_PROGRAM "C:/ProgramData/mingw64/mingw64/bin/mingw32-make.exe")
#set(CMAKE_RC_COMPILER "C:/ProgramData/mingw64/mingw64/bin/windres.exe")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Compiler flags
#if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
#elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
#endif()

# SDL3 configuration
list(APPEND CMAKE_PREFIX_PATH 
    "C:/Program Files (x86)/SDL3" 
    "C:/Program Files (x86)/SDL3_image"
)
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

# GLM - put back when you start using
# find_package(glm CONFIG REQUIRED)

# Add source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp" 
)
add_executable(low_rez ${SOURCES})

# Link libraries
target_link_libraries( 
    low_rez
    PRIVATE 
    SDL3::SDL3
    SDL3_image::SDL3_image
#    glm::glm
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
target_include_directories(
    low_rez
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

# Copy SDL3.dll to output directory
file(GLOB SDL3_DLLS "C:/Program Files (x86)/SDL3/*.dll")
foreach(DLL ${SDL3_DLLS})
    add_custom_command(TARGET low_rez POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL}
        $<TARGET_FILE_DIR:low_rez>)
endforeach()

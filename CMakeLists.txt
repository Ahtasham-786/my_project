# ==============================================================================
# SMART FILE MANAGEMENT SYSTEM - CMAKE BUILD CONFIGURATION
# ==============================================================================
# CMake is a cross-platform build system generator
# It generates platform-specific build files (Makefiles, Visual Studio projects, etc.)
# 
# Teaching Point: WHY USE CMAKE?
# - Cross-platform (works on Windows, Linux, macOS)
# - Handles dependencies automatically
# - Integrates with IDEs (VS Code, CLion, Visual Studio)
# - Industry standard for C++ projects
# ==============================================================================

# Minimum CMake version required
# Teaching Point: Version requirement ensures features we use are available
cmake_minimum_required(VERSION 3.17)

# Project name and version
# Teaching Point: PROJECT() command sets several variables:
# - PROJECT_NAME
# - PROJECT_VERSION
# - CMAKE_PROJECT_NAME (if this is the top-level CMakeLists.txt)
project(SmartFileManager 
        VERSION 1.0.0
        DESCRIPTION "Modern C++17 File Management System"
        LANGUAGES CXX)

# ==============================================================================
# C++ STANDARD CONFIGURATION
# ==============================================================================
# Teaching Point: C++ standards evolution:
# - C++11: auto, lambda, smart pointers
# - C++14: generic lambdas, relaxed constexpr
# - C++17: filesystem, structured bindings, if-init
# - C++20: concepts, ranges, coroutines

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions

# Teaching Point: What do these settings mean?
# CMAKE_CXX_STANDARD 17: Use C++17 features
# REQUIRED ON: Fail build if C++17 not available
# EXTENSIONS OFF: Use standard C++ only (no GNU extensions)

# ==============================================================================
# COMPILER FLAGS
# ==============================================================================
# Teaching Point: Compiler flags control warnings, optimizations, debugging

# Common flags for all compilers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Teaching Point: WHAT ARE THESE FLAGS?
# -Wall: Enable all common warnings
# -Wextra: Enable extra warnings
# -Wpedantic: Strict ISO C++ compliance warnings

# Platform-specific flags
if(MSVC)
    # Microsoft Visual C++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    # /W4 = Warning level 4 (highest reasonable level)
else()
    # GCC and Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -Wnon-virtual-dtor")
    # -Wshadow: Warn when variable shadows another
    # -Wnon-virtual-dtor: Warn about missing virtual destructors
endif()

# Debug vs Release configurations
# Teaching Point: BUILD TYPES
# - Debug: No optimization, debugging symbols, assertions enabled
# - Release: Full optimization, no debugging symbols, assertions disabled
# - RelWithDebInfo: Optimization + debugging symbols
# - MinSizeRel: Optimize for size

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "Build type not specified, defaulting to Debug")
endif()

# ==============================================================================
# SOURCE FILES
# ==============================================================================
# Teaching Point: ORGANIZING SOURCE FILES
# 
# We can specify files individually or use GLOB
# 
# INDIVIDUAL (more explicit, recommended):
set(SOURCES
    src/main.cpp
    src/Logger.cpp
    src/FileManager.cpp
    src/FileSorter.cpp
    src/FileSearcher.cpp
    src/Menu.cpp
)

# GLOB (automatic, but CMake won't detect new files without re-running):
# file(GLOB SOURCES "src/*.cpp")

# Header files (for IDE integration, not compilation)
set(HEADERS
    include/FileInfo.h
    include/Logger.h
    include/FileManager.h
    include/FileSorter.h
    include/FileSearcher.h
    include/Menu.h
)

# ==============================================================================
# EXECUTABLE TARGET
# ==============================================================================
# Teaching Point: TARGET = Build output (executable or library)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Teaching Point: What does add_executable() do?
# - Creates a build target named "SmartFileManager"
# - Compiles all source files
# - Links them into an executable
# - Executable name: SmartFileManager (or SmartFileManager.exe on Windows)

# ==============================================================================
# INCLUDE DIRECTORIES
# ==============================================================================
# Teaching Point: INCLUDE PATHS
# Tells compiler where to find header files

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Teaching Point: PRIVATE vs PUBLIC vs INTERFACE
# - PRIVATE: Only this target uses these includes
# - PUBLIC: This target and targets that link to it
# - INTERFACE: Only targets that link to this target
# 
# For executables, use PRIVATE (nothing links to an executable)

# ==============================================================================
# LINKING LIBRARIES
# ==============================================================================
# Teaching Point: LINKING
# Even though we don't use external libraries, we need filesystem

# Filesystem library linking
# Teaching Point: std::filesystem requires explicit linking on some platforms
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    # GCC < 9.0 requires explicit stdc++fs linking
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    # Clang < 9.0 requires explicit c++fs linking
    target_link_libraries(${PROJECT_NAME} PRIVATE c++fs)
endif()

# Thread library (for std::mutex in Logger)
# Teaching Point: Threading support
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# ==============================================================================
# INSTALLATION RULES (OPTIONAL)
# ==============================================================================
# Teaching Point: INSTALLATION
# Defines where to install files when running "make install"

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

# Install documentation
install(FILES README.md
        DESTINATION share/doc/${PROJECT_NAME}
)

# ==============================================================================
# OUTPUT INFORMATION
# ==============================================================================
# Teaching Point: STATUS MESSAGES
# Helpful for users to see configuration details

message(STATUS "")
message(STATUS "╔════════════════════════════════════════════════════════╗")
message(STATUS "║    Smart File Management System - Build Configuration  ║")
message(STATUS "╚════════════════════════════════════════════════════════╝")
message(STATUS "")
message(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "")
message(STATUS "Source directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Binary directory: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "")
message(STATUS "Build instructions:")
message(STATUS "  1. mkdir build && cd build")
message(STATUS "  2. cmake ..")
message(STATUS "  3. cmake --build .")
message(STATUS "  4. ./SmartFileManager [optional_directory_path]")
message(STATUS "")
message(STATUS "════════════════════════════════════════════════════════")
message(STATUS "")

# ==============================================================================
# KEY TAKEAWAYS - CMAKE BEST PRACTICES
# ==============================================================================
# 
# 1. MODERN CMAKE (3.0+):
#    - Use target_*() commands (target_include_directories, target_link_libraries)
#    - Avoid global commands (include_directories, link_libraries)
#    - Better for multi-target projects
# 
# 2. VERSION CONTROL:
#    - Specify minimum CMake version
#    - Document required C++ standard
#    - Handle platform differences
# 
# 3. COMPILER FLAGS:
#    - Enable warnings (-Wall -Wextra)
#    - Use platform-specific flags when needed
#    - Support Debug and Release builds
# 
# 4. ORGANIZATION:
#    - Clear section comments
#    - Logical grouping of related settings
#    - Status messages for user feedback
# 
# 5. PORTABILITY:
#    - Check compiler ID and version
#    - Handle platform-specific requirements
#    - Use CMake variables (not hardcoded paths)
# 
# 6. DEPENDENCIES:
#    - Explicit library linking
#    - find_package for external libraries
#    - Handle optional dependencies gracefully
# 
# ==============================================================================
# BUILD COMMANDS CHEAT SHEET
# ==============================================================================
# 
# BASIC BUILD:
#   mkdir build && cd build
#   cmake ..
#   cmake --build .
# 
# SPECIFY BUILD TYPE:
#   cmake -DCMAKE_BUILD_TYPE=Release ..
#   cmake -DCMAKE_BUILD_TYPE=Debug ..
# 
# SPECIFY COMPILER:
#   cmake -DCMAKE_CXX_COMPILER=g++ ..
#   cmake -DCMAKE_CXX_COMPILER=clang++ ..
# 
# VERBOSE BUILD (see full commands):
#   cmake --build . --verbose
#   OR
#   make VERBOSE=1
# 
# CLEAN BUILD:
#   cmake --build . --clean-first
# 
# INSTALL:
#   cmake --install .
#   OR
#   make install
# 
# ==============================================================================

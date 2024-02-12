cmake_minimum_required(VERSION 3.27.8)

project(memecache
        VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to install caches library
option(INSTALL_CACHES "Install caches library" OFF)

# Add your executables
add_executable(main main.cpp)
add_executable(mainthread mainthread.cpp)

# Define clang-format command
set(CLANG_FORMAT_COMMAND "clang-format")

# Find all source files excluding those in the "build" directory
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/*.cpp ${CMAKE_SOURCE_DIR}/*.hpp)
list(FILTER SOURCE_FILES EXCLUDE REGEX "build/.*")

# Add custom target for formatting
add_custom_target(format
    COMMAND ${CLANG_FORMAT_COMMAND} -i ${SOURCE_FILES}
    COMMENT "Running clang-format on all source files"
    VERBATIM
)

# Add dependency to run format target after build
add_dependencies(main format)
add_dependencies(mainthread format)

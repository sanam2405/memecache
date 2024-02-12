cmake_minimum_required(VERSION 3.27.8)

project(memecache
        VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(INSTALL_CACHES "Install caches library" OFF)

add_executable(main main.cpp)
add_executable(mainthread mainthread.cpp)

find_program(CLANG_FORMAT_COMMAND clang-format)

if(CLANG_FORMAT_COMMAND)
    file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/*.cpp ${CMAKE_SOURCE_DIR}/*.hpp)
    list(FILTER SOURCE_FILES EXCLUDE REGEX "build/.*")

    # Define custom target for formatting
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_COMMAND} -i ${SOURCE_FILES}
        COMMENT "Running clang-format on all source files"
        VERBATIM
    )

    # Add dependency on format target after building main and mainthread
    add_dependencies(main format)
    add_dependencies(mainthread format)
else()
    message(STATUS "clang-format command not found. Skipping format target.")
endif()
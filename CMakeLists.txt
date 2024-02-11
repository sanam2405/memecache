cmake_minimum_required(VERSION 3.27.8)

project(memecache
        VERSION 1.0.0)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(INSTALL_CACHES "Install caches library" OFF)

add_executable(main main.cpp)
add_executable(mainthread mainthread.cpp)



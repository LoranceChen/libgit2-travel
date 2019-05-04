cmake_minimum_required(VERSION 3.13)
project(libgit_test)

add_executable(libgit_test main.cpp)

set(CMAKE_CXX_STANDARD 14)

target_include_directories(libgit_test
        PUBLIC
        /usr/local/include
        )
target_link_libraries(libgit_test
        /usr/local/lib/libgit2.dylib
        )


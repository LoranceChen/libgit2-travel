cmake_minimum_required(VERSION 3.13)
project(libgit_test)

add_executable(libgit_test main.c wapper.h)

#set(CMAKE_CXX_STANDARD 14)
set_property(TARGET libgit_test PROPERTY C_STANDARD 90)
target_include_directories(libgit_test
        PUBLIC
        /usr/lib/clang/9.0.0/include
        /Users/lorancechen/version_control_project/_open_source/clang/libgit2/include
        )
target_link_libraries(libgit_test
        /Users/lorancechen/version_control_project/_open_source/clang/libgit2/build/libgit2.28.dylib
        )


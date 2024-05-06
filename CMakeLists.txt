cmake_minimum_required(VERSION 3.26)
project(colorconvert)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk+-3.0)

include_directories(${GTK_INCLUDE_DIRS})

link_libraries(${GTK_LIBRARIES})

add_executable(colorconvert main.cpp
                hex_table.hex)
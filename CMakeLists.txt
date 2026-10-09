cmake_minimum_required(VERSION 4.4.4)
project(waszeit)

set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
add_executable(waszeit src/main.c src/state.c src/database.c)

find_package(SQLite3)
target_link_libraries(waszeit PRIVATE SQLite3::SQLite3)

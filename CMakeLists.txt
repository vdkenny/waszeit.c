cmake_minimum_required(VERSION 4.4.4)
project(waszeit)

add_executable(waszeit src/main.c)

find_package(SQLite3)
target_link_libraries(waszeit PRIVATE SQLite3::SQLite3)

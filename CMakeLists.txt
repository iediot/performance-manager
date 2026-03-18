cmake_minimum_required(VERSION 3.16)
project(GtkApp)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_executable(app main.cpp)

target_link_libraries(app ${GTK_LIBRARIES})
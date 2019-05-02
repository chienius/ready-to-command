project(ready-to-command)
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)
add_subdirectory(src ready-to-command)

cmake_minimum_required(VERSION 3.13)


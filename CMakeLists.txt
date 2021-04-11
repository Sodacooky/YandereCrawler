cmake_minimum_required(VERSION 3.0.0)
set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic)
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
project(YandereCrawler)

aux_source_directory(. SRC_LIST)
add_executable(YandereCrawler ${SRC_LIST})

find_package(CURL CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE CURL::libcurl)
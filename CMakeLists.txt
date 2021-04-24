cmake_minimum_required(VERSION 3.0.0)
set(VCPKG_TARGET_TRIPLET x64-mingw-static)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
project(YandereCrawler)

# main.cpp
aux_source_directory(src SRC)
add_executable(${CMAKE_PROJECT_NAME} ${SRC})

# curl
find_package(CURL CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE CURL::libcurl)
# openssl curl depend
find_package(OpenSSL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
# spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
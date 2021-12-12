# maybe unnecessary
cmake_minimum_required(VERSION 3.0.0)
# vcpkg setting
if (DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_TARGET_TRIPLET x64-windows)
    set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
endif ()
# c++17 enable
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Project name: Proj
project(YandereCrawler VERSION 0.5)

aux_source_directory(./src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

find_package(cpr CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)

find_package(teemo CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE teemo)

find_package(OpenSSL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

find_package(ZLIB REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ZLIB::ZLIB)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
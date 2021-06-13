cmake_minimum_required(VERSION 3.0.0)
set(VCPKG_TARGET_TRIPLET x64-windows)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
project(Proj VERSION 0.3)

aux_source_directory(./src SRC_LIST)
add_executable(Proj ${SRC_LIST})

find_package(CURL CONFIG REQUIRED)
target_link_libraries(Proj PRIVATE CURL::libcurl)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(Proj PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(Proj PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
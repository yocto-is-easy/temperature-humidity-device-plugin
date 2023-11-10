project(temperature-humidity-device-plugin)

cmake_minimum_required(VERSION 3.10)

set(SERVICE_NAME ${CMAKE_PROJECT_NAME}-service)

include_directories(
    include/
)

add_executable(${SERVICE_NAME}
    impl/service/src/main.cpp
)

target_include_directories(${SERVICE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/service/include
)

find_package(Threads REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

target_link_libraries(${SERVICE_NAME}
    Threads::Threads
    spdlog::spdlog_header_only
    Boost::system
)

target_include_directories( ${SERVICE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/service/include
)

target_compile_features(${SERVICE_NAME} PRIVATE cxx_std_17)

cmake_minimum_required(VERSION 3.20)
project(DriverLife VERSION 0.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(DriverLife SHARED
    src/DriverLife.cpp
)
target_include_directories(DriverLife PRIVATE include)
target_compile_definitions(DriverLife PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
set_target_properties(DriverLife PROPERTIES OUTPUT_NAME "DriverLife")

add_executable(DriverLifeOverlay WIN32
    src/Overlay.cpp
)
target_include_directories(DriverLifeOverlay PRIVATE include)
target_compile_definitions(DriverLifeOverlay PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
target_link_libraries(DriverLifeOverlay PRIVATE user32 gdi32)
set_target_properties(DriverLifeOverlay PROPERTIES OUTPUT_NAME "DriverLifeOverlay")

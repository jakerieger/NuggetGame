cmake_minimum_required(VERSION 3.28)
project(XEN-Pak)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/XPak)

include_directories(
        ./
        ../XEN-PlatformTools
        ../ThirdParty
)

find_package(liblzma CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(crossguid CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(AudioFile CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

add_library(XPak-Core STATIC XPak.cpp XPak.h ../ThirdParty/stb_image.h ../ThirdParty/stb_image.cpp)
target_link_libraries(XPak-Core PRIVATE XPlatform-Core liblzma::liblzma yaml-cpp::yaml-cpp crossguid fmt::fmt AudioFile)

add_executable(XPak main.cpp)
target_link_libraries(XPak PRIVATE XPak-Core CLI11::CLI11 yaml-cpp::yaml-cpp)

add_custom_command(TARGET XPak PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets)
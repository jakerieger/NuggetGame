cmake_minimum_required(VERSION 3.20...3.28)

project(AssetTool)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/AssetTool)

include_directories(
        ../XEN-Core
        ../XEN-AssetTool
        ../XEN-PlatformTools
        ../ThirdParty
)

add_library(XEN-AssetCore STATIC AssetDescriptor.cpp AssetDescriptor.h
        AssetManifest.cpp
        AssetManifest.h)
target_link_libraries(XEN-AssetCore PRIVATE XEN-PlatformCore rapidjson)
add_executable(AssetTool main.cpp)
target_link_libraries(AssetTool PRIVATE XEN-AssetCore rapidjson)

add_custom_command(TARGET AssetTool PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/XEN-AssetTool/Sprites.manifest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Sprites.manifest)
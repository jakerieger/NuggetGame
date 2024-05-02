cmake_minimum_required(VERSION 3.20...3.28)

project(AssetTool)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/AssetTool)

include_directories(
        ../XEN-Core
        ../XEN-AssetTool
        ../XEN-PlatformTools
        ../ThirdParty
)

add_library(XEN-AssetCore STATIC AssetDescriptor.cpp AssetDescriptor.h
        AssetManifest.cpp
        AssetManifest.h
        AssetPak.cpp
        AssetPak.h)
target_link_libraries(XEN-AssetCore PRIVATE XEN-PlatformCore rapidjson liblzma::liblzma)
add_executable(AssetTool main.cpp)
target_link_libraries(AssetTool PRIVATE XEN-AssetCore rapidjson liblzma::liblzma)

add_custom_command(TARGET AssetTool PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/XEN-AssetTool/Sprites.manifest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Sprites.manifest)
add_custom_command(TARGET AssetTool PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/XEN-AssetTool/Fonts.manifest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Fonts.manifest)
add_custom_command(TARGET AssetTool PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/XEN-AssetTool/Audio.manifest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Audio.manifest)
add_custom_command(TARGET AssetTool PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/XEN-AssetTool/Levels.manifest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Levels.manifest)
add_custom_command(TARGET AssetTool PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/XEN-AssetTool/manifests.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/manifests.json)
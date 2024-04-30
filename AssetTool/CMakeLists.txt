cmake_minimum_required(VERSION 3.28)

project(AssetTool)

add_library(AssetToolCore STATIC Lib/AssetDescriptor.cpp Lib/AssetDescriptor.h)
add_executable(AssetTool App/main.cpp)
target_link_libraries(AssetTool PRIVATE AssetToolCore)
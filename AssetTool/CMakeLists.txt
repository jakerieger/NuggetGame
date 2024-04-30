cmake_minimum_required(VERSION 3.28)

project(AssetTool)

include_directories(Lib)

add_library(AssetToolCore STATIC AssetDescriptor.cpp AssetDescriptor.h)
add_executable(AssetTool main.cpp)
target_link_libraries(AssetTool PRIVATE AssetToolCore)
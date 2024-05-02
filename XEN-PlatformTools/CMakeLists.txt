cmake_minimum_required(VERSION 3.20...3.28)

project(PlatformTools)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(XEN-PlatformCore STATIC PlatformTools.h IO.cpp IO.h)
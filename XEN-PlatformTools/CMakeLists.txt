cmake_minimum_required(VERSION 3.28)

project(PlatformTools)

add_library(XEN-PlatformCore STATIC PlatformTools.h IO.cpp IO.h)
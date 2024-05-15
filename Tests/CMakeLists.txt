cmake_minimum_required(VERSION 3.20...3.28)
project(Tests)

include_directories(
        ../XEN-AssetTool
        ../XEN-Core
        ../XEN-PlatformTools
        ../ThirdParty
)

find_package(Catch2 CONFIG REQUIRED)

add_subdirectory(NuggetGame)
add_subdirectory(XEN-AssetTool)
add_subdirectory(XEN-Core)
add_subdirectory(XEN-PlatformTools)

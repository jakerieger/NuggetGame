cmake_minimum_required(VERSION 3.20...3.28)

project(NuggetGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/NuggetGame)

include_directories(
        ../XEN-Core
        ../XEN-Pak
        ../XEN-PlatformTools
        ../ThirdParty
        ../NuggetGame
)

add_definitions(
        -D_CRT_SECURE_NO_WARNINGS
        -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
)

file(GLOB RMLUI_SRCS ../ThirdParty/RmlUi/*.cpp)

add_executable(NuggetGame
        ${RMLUI_SRCS}
        Res/app.rc
        main.cpp
        Res/resource.h
        Nugget.cpp
        Nugget.h
        Level.cpp
        Level.h
        Levels.h
        UI/MainMenu.h
        UI/MainMenu.cpp
        UI/SettingsMenu.h
        UI/SettingsMenu.cpp
)
target_link_libraries(NuggetGame PRIVATE
        XCore
        XPlatform-Core
        XPak-Core
        glfw
        liblzma::liblzma
        RmlCore
        RmlDebugger
        fmt::fmt
)

add_custom_command(TARGET NuggetGame PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/Assets/bitmaps/nugget.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/icon.png)
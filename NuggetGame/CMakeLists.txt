cmake_minimum_required(VERSION 3.28)

project(NuggetGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        ../XEN-Core
        ../XEN-AssetTool
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
        XEN-Core
        XEN-PlatformTools
        XEN-AssetCore
        glfw
        lz4::lz4
        RmlCore
        RmlDebugger
        fmt::fmt
)

add_custom_command(TARGET NuggetGame PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/Assets/sprites/source_imgs/nugget.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/icon.png)
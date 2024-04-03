cmake_minimum_required(VERSION 3.28)
project(NuggetGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        Core
        Lib
        Game
        Packer
        imgui
        imgui/backends
)

add_definitions(
        -D_CRT_SECURE_NO_WARNINGS
        -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
)

file(GLOB IMGUI_SRCS imgui/*.cpp imgui/backends/imgui_impl_opengl3.cpp imgui/backends/imgui_impl_glfw.cpp)
file(GLOB RMLUI_SRCS Lib/RmlUi/*.cpp)

add_library(Core STATIC
        Lib/glad.c
        Lib/stb_image.cpp
        ${IMGUI_SRCS}
        ${RMLUI_SRCS}
        Core/Engine/Profiler.cpp
        Core/Engine/Profiler.h
        Core/Engine/DebugUI.cpp
        Core/Engine/DebugUI.h
        Core/Shader.cpp
        Core/Engine/GameApp.cpp
        Core/Engine/GameApp.h
        Core/Engine/GraphicsContext.cpp
        Core/Engine/GraphicsContext.h
        Core/Utilities.inl
        Core/STL.h
        Core/Engine/Resources.h
        Core/STL.cpp
        Core/Engine/Input.cpp
        Core/Engine/Input.h
        Core/Engine/Resources.cpp
        Core/Engine/GraphicsError.cpp
        Core/Engine/GraphicsError.h
        Core/Engine/Profiler.cpp
        Core/Engine/Profiler.h
        Core/Components/Transform.cpp
        Core/Components/Transform.h
        Core/Scene.cpp
        Core/Scene.h
        Core/SceneContext.cpp
        Core/SceneContext.h
        Core/Interfaces/GameObject.h
        Core/Interfaces/Component.h
        Core/Interfaces/Lifetime.h
        Core/Interfaces/Drawable.h
        Core/Interfaces/InputListener.h
        Core/Engine/InputEvents.h
        Core/Engine/InputCodes.h
        Core/Types.h
        Core/Interfaces/RenderPass.h
        Core/Sprite.h
        Core/SpriteBatch.h
        Core/Components/SpriteRenderer.cpp
        Core/Components/SpriteRenderer.h
        Core/Color.cpp
        Core/Color.h
        Core/Components/Rigidbody.cpp
        Core/Components/Rigidbody.h
        Core/Engine/PhysicsContext.cpp
        Core/Engine/PhysicsContext.h
        Core/Components/Tilemap.cpp
        Core/Components/Tilemap.h
        Core/Primitives.h
        Core/Engine/DebugPhysics.cpp
        Core/Engine/DebugPhysics.h
        Core/GameObject.h
        Core/Engine/GameUI.cpp
        Core/Engine/GameUI.h
        Core/Camera.cpp
        Core/Camera.h
        Core/Logger.cpp
        Core/Logger.h
        Core/Engine/AudioContext.cpp
        Core/Engine/AudioContext.h
        Core/Level.cpp
        Core/Level.h
)

# ============[ PACKAGES ]============ #
find_package(glfw3 CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(RmlUi CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
# ============++++++++++++============ #

target_link_libraries(Core PRIVATE
        glfw
        box2d::box2d
        RmlCore
        RmlDebugger
        fmt::fmt
        OpenAL::OpenAL
)

# NugPak
add_library(Packer STATIC
        Packer/Packer.cpp
        Packer/Unpacker.cpp
        Packer/Packer.h
        Packer/Unpacker.h
        Lib/stb_image.cpp)

find_package(lz4 CONFIG REQUIRED)
target_link_libraries(Packer PRIVATE lz4::lz4)

add_executable(NugPak Packer/NugPak.cpp)
target_link_libraries(NugPak PRIVATE Packer lz4::lz4)

add_executable(NuggetGame
        ${RMLUI_SRCS}
        Game/Res/app.rc
        Game/main.cpp
        Game/Res/resource.h
        Game/Nugget.cpp
        Game/Nugget.h
        Game/Floor.cpp
        Game/Floor.h
)
target_link_libraries(NuggetGame PRIVATE
        Core
        glfw
        Packer
        lz4::lz4
        RmlCore
        RmlDebugger
        fmt::fmt
)

add_custom_command(TARGET Packer PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_BINARY_DIR}/Assets)
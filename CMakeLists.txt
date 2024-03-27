cmake_minimum_required(VERSION 3.28)
project(NuggetGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        Core
        Lib
        Game
        Packer
)

add_definitions(
        -D_CRT_SECURE_NO_WARNINGS
        -D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
)

add_library(Core STATIC
        Lib/glad.c
        Lib/stb_image.cpp
        Core/Shader.cpp
        Core/Engine/GameApp.cpp
        Core/Engine/GameApp.h
        Core/Engine/GraphicsContext.cpp
        Core/Engine/GraphicsContext.h
        Core/Utilities.inl
        Core/STL.h
        Core/Engine/Resources.h
        Core/STL.cpp
        Core/Input.cpp
        Core/Input.h
        Core/Engine/Resources.cpp
        Core/Engine/GraphicsError.cpp
        Core/Engine/GraphicsError.h
        Core/Engine/Profiler.cpp
        Core/Engine/Profiler.h
        Core/Transform.cpp
        Core/Transform.h
        Core/Scene.cpp
        Core/Scene.h
        Core/SceneContext.cpp
        Core/SceneContext.h
        Core/Interfaces/GameObject.h
        Core/Interfaces/Component.h
        Core/Interfaces/Lifetime.h
        Core/Interfaces/Drawable.h
        Core/Interfaces/InputListener.h
        Core/InputEvents.h
        Core/InputCodes.h
        Core/Types.h
        Core/Framebuffer.cpp
        Core/Framebuffer.h
        Core/Interfaces/RenderPass.h
        Core/Sprite.h
        Core/SpriteRenderer.cpp
        Core/SpriteRenderer.h
        Core/Color.cpp
        Core/Color.h
        Core/Rigidbody.cpp
        Core/Rigidbody.h
        Core/Engine/PhysicsContext.cpp
        Core/Engine/PhysicsContext.h
        Core/Tilemap.cpp
        Core/Tilemap.h
)

find_package(glfw3 CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
target_link_libraries(Core PRIVATE glfw box2d::box2d)

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

add_executable(NuggetGame Game/Res/app.rc Game/main.cpp
        Game/Res/resource.h
        Game/Nugget.cpp
        Game/Nugget.h
        Game/Floor.cpp
        Game/Floor.h
)
target_link_libraries(NuggetGame PRIVATE Core glfw Packer lz4::lz4)

add_custom_command(TARGET Packer PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_BINARY_DIR}/Assets)
cmake_minimum_required(VERSION 3.28)
project(NuggetGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        Core
        Lib
        Game
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
)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Core PRIVATE glfw)

add_executable(NuggetGame Game/app.rc Game/main.cpp
        Game/resource.h
        Game/Nugget.cpp
        Game/Nugget.h)
target_link_libraries(NuggetGame PRIVATE Core glfw)

add_custom_command(TARGET NuggetGame PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_BINARY_DIR}/Assets)

#find_package(libdeflate CONFIG REQUIRED)
#target_link_libraries(main PRIVATE $<IF:$<TARGET_EXISTS:libdeflate::libdeflate_shared>,libdeflate::libdeflate_shared,libdeflate::libdeflate_static>)
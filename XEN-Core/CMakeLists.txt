cmake_minimum_required(VERSION 3.20...3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

add_library(XCore STATIC
        ../ThirdParty/glad.c
        ../ThirdParty/stb_image.cpp
        ${RMLUI_SRCS}
        Engine/Profiler.cpp
        Engine/Profiler.h
        Shader.cpp
        Engine/GameApp.cpp
        Engine/GameApp.h
        Engine/GraphicsContext.cpp
        Engine/GraphicsContext.h
        Utilities.inl
        STL.h
        Engine/Resources.h
        STL.cpp
        Engine/Input.cpp
        Engine/Input.h
        Engine/Resources.cpp
        Engine/GraphicsError.cpp
        Engine/GraphicsError.h
        Engine/Profiler.cpp
        Engine/Profiler.h
        Components/Transform.cpp
        Components/Transform.h
        Scene.cpp
        Scene.h
        SceneContext.cpp
        SceneContext.h
        Interfaces/GameObject.h
        Interfaces/Component.h
        Interfaces/Lifetime.h
        Interfaces/Drawable.h
        Interfaces/InputListener.h
        Engine/InputEvents.h
        Engine/InputCodes.h
        Types.h
        Interfaces/RenderPass.h
        Sprite.h
        SpriteBatch.h
        Components/SpriteRenderer.cpp
        Components/SpriteRenderer.h
        Color.cpp
        Color.h
        Components/Rigidbody.cpp
        Components/Rigidbody.h
        Engine/PhysicsContext.cpp
        Engine/PhysicsContext.h
        Components/Tilemap.cpp
        Components/Tilemap.h
        Primitives.h
        Engine/DebugPhysics.cpp
        Engine/DebugPhysics.h
        GameObject.h
        Engine/GameUI.cpp
        Engine/GameUI.h
        Camera.cpp
        Camera.h
        Logger.cpp
        Logger.h
        Engine/AudioContext.cpp
        Engine/AudioContext.h
        Engine/EngineSettings.h
        Engine/EngineSettings.cpp
        NativePlatform.h
        Interfaces/UIDocument.h
        Interfaces/UIListener.h
        Cursor.h
        Cursor.cpp
        Interfaces/Serializable.h
        Movie.cpp
        Movie.h
)

target_link_libraries(XCore PRIVATE
        glfw
        box2d::box2d
        RmlCore
        RmlDebugger
        fmt::fmt
        OpenAL::OpenAL
        XPlatform-Core
        XPak-Core
)
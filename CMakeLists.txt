cmake_minimum_required(VERSION 3.20...3.28)
project(NuggetGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RELEASE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libraries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)

include_directories(
        XEN-Core
        ThirdParty
        NuggetGame
)

# ============[ PACKAGES ]============ #
find_package(glfw3 CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(RmlUi CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(liblzma CONFIG REQUIRED)
# ============++++++++++++============ #

add_subdirectory(XEN-Core)
add_subdirectory(XEN-Pak)
add_subdirectory(XEN-PlatformTools)
add_subdirectory(NuggetGame)
add_subdirectory(Tests)

# ================================================================================================================== #
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/NuggetGame/UI)
set(OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/NuggetGame/Interface)

set(FILE_EXTENSIONS "*.html" "*.css" "*.tga")
set(FILES_TO_COPY)

foreach (EXTENSION ${FILE_EXTENSIONS})
    file(GLOB FILES "${SOURCE_DIR}/${EXTENSION}")
    list(APPEND FILES_TO_COPY ${FILES})
endforeach ()

file(COPY ${FILES_TO_COPY} DESTINATION ${OUTPUT_DIR})
file(MAKE_DIRECTORY ${OUTPUT_DIR})

add_custom_target(
        CopyUiFiles ALL
        DEPENDS ${OUTPUT_DIR}
        COMMENT "Copying files to output directory"
)

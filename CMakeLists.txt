cmake_minimum_required(VERSION 3.28)
project(NuggetGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
find_package(lz4 CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
# ============++++++++++++============ #

add_subdirectory(XEN-Core)
add_subdirectory(XEN-AssetTool)
add_subdirectory(XEN-PlatformTools)
add_subdirectory(NuggetGame)

# ================================================================================================================== #
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/Game/UI)
set(OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Interface)

# Define the file extensions you want to copy
set(FILE_EXTENSIONS "*.html" "*.css" "*.tga")

# Create a list to hold files to be copied
set(FILES_TO_COPY)

# Add files matching the extensions to the list
foreach (EXTENSION ${FILE_EXTENSIONS})
    file(GLOB FILES "${SOURCE_DIR}/${EXTENSION}")
    list(APPEND FILES_TO_COPY ${FILES})
endforeach ()

# Copy files to the output directory
file(COPY ${FILES_TO_COPY} DESTINATION ${OUTPUT_DIR})

# Make sure to create the output directory
file(MAKE_DIRECTORY ${OUTPUT_DIR})

# Create a custom target depending on the existence of the output files
add_custom_target(
        copy_files_target ALL
        DEPENDS ${OUTPUT_DIR}
        COMMENT "Copying files to output directory"
)

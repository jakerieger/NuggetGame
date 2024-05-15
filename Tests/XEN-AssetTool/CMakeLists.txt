cmake_minimum_required(VERSION 3.20...3.28)
project(XEN-AssetTool-Tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Tests/AssetTool)

add_executable(Test.AssetMetadata AssetMetadata.test.cpp)
add_executable(Test.AssetDescriptor AssetDescriptor.test.cpp)
add_executable(Test.AssetManifest AssetManifest.test.cpp)
add_executable(Test.AssetPak AssetPak.test.cpp)

set(TEST_TARGETS
        Test.AssetMetadata
        Test.AssetDescriptor
        Test.AssetManifest
        Test.AssetPak
)

foreach (target ${TEST_TARGETS})
    target_link_libraries(${target} PRIVATE XEN-AssetCore Catch2::Catch2WithMain)
endforeach ()

add_custom_target(Test.AssetTool DEPENDS ${TEST_TARGETS})
add_custom_command(
        TARGET Test.AssetTool POST_BUILD
        COMMAND Test.AssetMetadata
        COMMAND Test.AssetDescriptor
        COMMAND Test.AssetManifest
        COMMAND Test.AssetPak
)
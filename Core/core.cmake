cmake_minimum_required(VERSION 3.22)

add_library(${PROJECT_NAME} Core/tlv.c)
target_include_directories(${PROJECT_NAME} PUBLIC Core)

# Use C11
target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)

# Enable all warnings + treat warnings as errors
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /WX
    )
endif()
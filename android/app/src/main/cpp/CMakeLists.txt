cmake_minimum_required(VERSION 3.22.1)

project("universalbasic")

# Define paths to core source files
set(CORE_DIR ${CMAKE_SOURCE_DIR}/../../../../../src/core)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../../../../../include)

# Add include directories
include_directories(${INCLUDE_DIR})

# Core BASIC sources (platform-agnostic)
set(CORE_SOURCES
    ${CORE_DIR}/tokenizer.c
    ${CORE_DIR}/parser.c
    ${CORE_DIR}/runtime.c
    ${CORE_DIR}/program.c
)

# Android-specific sources
set(ANDROID_SOURCES
    pal_android.c
    native-lib.c
)

# Create shared library
add_library(${CMAKE_PROJECT_NAME} SHARED
    ${CORE_SOURCES}
    ${ANDROID_SOURCES}
)

# Link required libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    android
    log
    m
)

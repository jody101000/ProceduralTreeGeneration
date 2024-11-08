cmake_minimum_required(VERSION 3.10)
project(ProceduralTreeGeneration)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable folders for Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Add header files
file(GLOB_RECURSE HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Define resource directory path
target_compile_definitions(${PROJECT_NAME} PRIVATE
    RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resource/"
)

# Platform-specific configurations
if(APPLE)
    # Enable high-DPI support on macOS
    target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)
    
    # Find required frameworks
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    
    # Link frameworks
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()

# Set Visual Studio specific properties (Windows only)
if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

# Create source groups for better organization in Visual Studio
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES} ${HEADERS})

# Add external/glad/src/glad.c to sources
target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c")

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
)

# Find GLFW
# Handle GLFW for both platforms
if(WIN32)
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/lib-vc2022
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw3
        opengl32
    )
    
    # Copy GLFW DLL for Windows
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/lib-vc2022/glfw3.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/lib-vc2022/glfw3.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        )
    endif()
else()
    # Use bundled GLFW for macOS
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/lib-universal
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/lib-universal/libglfw.3.dylib
    )
    
    # Copy GLFW dylib for macOS
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/lib-universal/libglfw.3.dylib"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
endif()

# Copy resources to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resource"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resource"
)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
cmake_minimum_required(VERSION 3.15)
project(mdv LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use system-installed raylib
find_package(raylib REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Vector3D.cpp
    src/StateVector.cpp
    src/Integrator.cpp
    src/OrbitPropagator.cpp
    src/OrbitalElements.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Platform-specific settings
if (WIN32)
    target_link_libraries(${PROJECT_NAME} winmm)
endif()

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()
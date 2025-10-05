cmake_minimum_required(VERSION 3.15)
project(mdv)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/Vector3D.cpp
    src/StateVector.cpp
    src/Integrator.cpp
    src/OrbitPropagator.cpp
)

# Create executable
add_executable(mdv ${SOURCES})

# Link standard library explicitly
target_link_libraries(mdv -static-libgcc -static-libstdc++)
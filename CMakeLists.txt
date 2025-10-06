cmake_minimum_required(VERSION 3.15)
project(mdv LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    src/OrbitPresets.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

if (WIN32)
    # Include and link directories
    target_include_directories(${PROJECT_NAME} PRIVATE 
        C:/msys64/mingw64/include
    )
    link_directories(C:/msys64/mingw64/lib)

    
    target_link_libraries(${PROJECT_NAME}
    raylib
    glfw3
    opengl32
    gdi32
    winmm
)

endif()

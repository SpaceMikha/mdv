cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

project(mdv LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core sources
set(CORE_SOURCES
    src/core/Vector3D.cpp
    src/core/StateVector.cpp
    src/core/OrbitalElements.cpp
    src/core/OrbitPresets.cpp
)

# Simulation sources
set(SIMULATION_SOURCES
    src/simulation/Integrator.cpp
    src/simulation/OrbitPropagator.cpp
    src/simulation/Satellite.cpp
    src/simulation/Eclipse.cpp
    src/simulation/SolarAnalysis.cpp
    src/simulation/GroundTrack.cpp
    src/rendering/GroundTrackRenderer.cpp
)

# Rendering sources
set(RENDERING_SOURCES
    src/rendering/RenderUtils.cpp
    src/rendering/EarthRenderer.cpp
    src/rendering/GridRenderer.cpp
    src/rendering/OrbitRenderer.cpp
)

# Camera sources
set(CAMERA_SOURCES
    src/camera/CameraController.cpp
)

# UI sources
set(UI_SOURCES
    src/ui/FontSystem.cpp
    src/ui/UIManager.cpp
)

# Input sources
set(INPUT_SOURCES
    src/input/InputHandler.cpp
)

# Main executable
add_executable(${PROJECT_NAME}
    main.cpp
    ${CORE_SOURCES}
    ${SIMULATION_SOURCES}
    ${RENDERING_SOURCES}
    ${CAMERA_SOURCES}
    ${UI_SOURCES}
    ${INPUT_SOURCES}
)

# Include directories for all modules
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/simulation
    ${CMAKE_SOURCE_DIR}/src/rendering
    ${CMAKE_SOURCE_DIR}/src/camera
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/input
    ${CMAKE_SOURCE_DIR}/include
    C:/msys64/mingw64/include
)

if (WIN32)
    link_directories(C:/msys64/mingw64/lib)
    
    target_link_libraries(${PROJECT_NAME}
        raylib
        glfw3
        opengl32
        gdi32
        winmm
    )
endif()

# Print configuration info
message(STATUS "MDV Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Source Directory: ${CMAKE_SOURCE_DIR}")
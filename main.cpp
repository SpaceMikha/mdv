#include <iostream>
#include <vector>
#include "raylib.h"
#include "RenderUtils.h"

// Core
#include "Constants.h"
#include "Vector3D.h"
#include "StateVector.h"
#include "OrbitalElements.h"
#include "OrbitPresets.h"
#include "OrbitPropagator.h"

// Simulation
#include "Satellite.h"
#include "Eclipse.h"
#include "SolarAnalysis.h"

// Rendering
#include "EarthRenderer.h"
#include "GridRenderer.h"
#include "OrbitRenderer.h"

// Camera
#include "CameraController.h"

// UI
#include "FontSystem.h"
#include "UIManager.h"

// Input
#include "InputHandler.h"

int main() {
    // Window setup
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "MDV - Mission Design Visualizer");
    SetTargetFPS(60);
    
    // Initialize systems
    FontSystem fonts;
    fonts.load();
    
    EarthRenderer earth;
    earth.load();
    
    CameraController camera;
    camera.initialize();
    
    UIManager ui(screenWidth, screenHeight);
    InputHandler input;
    
    // Create satellites
    OrbitPropagator propagator(MU_EARTH);
    std::vector<OrbitPreset> presets = OrbitPresets::getAllPresets(MU_EARTH);
    
    std::vector<Satellite> satellites;
    for (const auto& preset : presets) {
        std::vector<StateVector> orbit = propagator.propagate(
            preset.initialState,
            preset.period,
            preset.period / 100.0
        );
        satellites.push_back(Satellite(preset, orbit));
        
        std::cout << "Orbit created: " << preset.name
                  << " [" << satellites.back().getStats().orbitFamily << "] "
                  << "(" << orbit.size() << " points)\n";
    }
    
    // Start all satellites hidden
    for (auto& sat : satellites) {
        sat.setVisible(false);
    }
    
    // Simulation state
    size_t activeSatellite = 0;
    float animationSpeed = 1.0f;
    float animationAccumulator = 0.0f;
    bool showGrids = true;
    bool earthRotation = true;
    Vector3D sunDirection(1.0, 0.0, 0.0);  // Sun along +X axis
    
    std::cout << "\n=== MDV Started ===\n";
    std::cout << "Press X for keyboard shortcuts\n";
    
    // ========== MAIN LOOP ==========
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // ===== UPDATE =====
        
        // Process input
        input.processInput(
            satellites,
            activeSatellite,
            camera,
            ui,
            animationSpeed,
            showGrids,
            earthRotation
        );
        
        // Update Earth
        if (earthRotation) {
            earth.update(deltaTime);
        }
        
        // Update animation
        if (animationSpeed > 0.01f) {
            animationAccumulator += animationSpeed;
            size_t framesToAdvance = static_cast<size_t>(animationAccumulator);
            animationAccumulator -= framesToAdvance;
            
            for (auto& sat : satellites) {
                sat.advanceFrames(framesToAdvance);
            }
        }
        
        // Update camera
        camera.update(deltaTime, satellites, activeSatellite);
        camera.handleManualControls();
        
        // Calculate orbital elements for active satellite
        OrbitalElements currentElements = OrbitalElements::fromStateVector(
            satellites[activeSatellite].getCurrentState(),
            MU_EARTH
        );
        
        // ===== DRAW =====
        
        BeginDrawing();
        ClearBackground(Color{10, 3, 15});
        
        // 3D Scene
        BeginMode3D(camera.getCamera());
        
            // Draw grids and axes
            if (showGrids) {
                GridRenderer::drawEquatorialGrid();
                GridRenderer::drawReferenceCircles();
                GridRenderer::drawOrbitalPlane(
                    satellites[activeSatellite].getOrbit(),
                    satellites[activeSatellite].getStats().familyColor
                );
            }
            
            // Draw Earth
            earth.draw();
            
            // Draw coordinate axes
            GridRenderer::drawCoordinateAxes(10.0f);
            
            // Draw all satellites
            OrbitRenderer::drawSatellites(
                satellites,
                activeSatellite,
                sunDirection,
                ui.isShowingEclipse()
            );
            
        EndMode3D();
        
        // 2D UI
        ui.draw(
            fonts,
            satellites,
            activeSatellite,
            currentElements,
            animationSpeed,
            showGrids,
            earthRotation,
            camera.isFollowModeEnabled(),
            GetFPS(),
            sunDirection
        );
        
        EndDrawing();
    }
    
    // Cleanup
    earth.unload();
    fonts.unload();
    CloseWindow();
    
    return 0;
}
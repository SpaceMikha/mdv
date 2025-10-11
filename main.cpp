#include "raylib.h"
#include "Constants.h"
#include "OrbitPropagator.h"
#include "Satellite.h"
#include "OrbitPresets.h"
#include "OrbitalElements.h"
#include "Eclipse.h"
#include "SolarAnalysis.h"
#include "EarthRenderer.h"
#include "GridRenderer.h"
#include "OrbitRenderer.h"
#include "CameraController.h"
#include "FontSystem.h"
#include "UIManager.h"
#include "InputHandler.h"
#include "RenderUtils.h"
#include <vector>
#include <iostream>

int main() {
    // Window setup
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Mission Design Visualizer v0.8.2");
    SetTargetFPS(60);
    
    // Initialize systems
    FontSystem fonts;
    fonts.load();
    
    EarthRenderer earth;
    earth.load();
    
    CameraController cameraController;
    cameraController.initialize();
    
    UIManager ui(screenWidth, screenHeight);
    InputHandler input;
    
    // Sun direction (fixed along +X axis for now)
    Vector3D sunDirection(1.0, 0.0, 0.0);
    
    // Create orbit propagator
    OrbitPropagator propagator(MU_EARTH);
    
    // Generate all preset orbits
    std::vector<OrbitPreset> presets = OrbitPresets::getAllPresets(MU_EARTH);
    std::vector<Satellite> satellites;
    
    std::cout << "Generating orbits...\n";
    for (const auto& preset : presets) {
        double timestep = preset.period / 360.0;
        auto trajectory = propagator.propagate(preset.initialState, preset.period, timestep);
        satellites.push_back(Satellite(preset, trajectory));
        std::cout << "  " << preset.name << ": " << trajectory.size() << " points\n";
    }
    
    // Start with all satellites hidden
    for (auto& sat : satellites) {
        sat.setVisible(false);
    }
    if (!satellites.empty()) {
        satellites[0].setVisible(true);
    }
    
    // Simulation state
    size_t activeSatelliteIndex = 0;
    float animationSpeed = 1.0f;
    bool showGrids = true;
    bool earthRotation = true;  // Add this variable
    
    std::cout << "\nVisualization ready!\n";
    std::cout << "Controls:\n";
    std::cout << "  SPACE: Pause/Resume\n";
    std::cout << "  X: Show help\n";
    std::cout << "  C: Toggle left sidebar (Satellites/Solar)\n";
    std::cout << "  E: Toggle right sidebar (Orbital Elements)\n";
    std::cout << "  V: Toggle eclipse visualization\n";
    std::cout << "  Y: Toggle solar panel analysis\n";
    
    // Main loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // Process input
        input.processInput(
            satellites, 
            activeSatelliteIndex, 
            cameraController, 
            ui,
            animationSpeed, 
            showGrids, 
            earthRotation  // FIXED: Use variable instead of function call
        );
        
        // Update Earth rotation state from input
        earth.setRotationEnabled(earthRotation);
        
        // Update systems
        earth.update(deltaTime);
        cameraController.update(deltaTime, satellites, activeSatelliteIndex);
        cameraController.handleManualControls();
        ui.update(deltaTime); // NEW: Update UI animations
        
        // Advance satellite animations
        if (animationSpeed > 0.01f) {
            size_t framesToAdvance = static_cast<size_t>(animationSpeed);
            if (framesToAdvance < 1) framesToAdvance = 1;
            
            for (auto& sat : satellites) {
                sat.advanceFrames(framesToAdvance);
            }
        }
        
        // Get current orbital elements for active satellite
        OrbitalElements currentElements;
        if (activeSatelliteIndex < satellites.size()) {
            currentElements = OrbitalElements::fromStateVector(
                satellites[activeSatelliteIndex].getCurrentState(),
                MU_EARTH
            );
        }
        
        // Rendering
        BeginDrawing();
        ClearBackground(Color{15, 23, 42, 255}); // UITheme::BG_DARK
        
        // 3D rendering
        BeginMode3D(cameraController.getCamera());
            
            earth.draw();
            
            if (showGrids) {
                GridRenderer::drawEquatorialGrid();
                GridRenderer::drawReferenceCircles();
            }
            
            OrbitRenderer::drawSatellites(
                satellites, 
                activeSatelliteIndex, 
                sunDirection,
                ui.isShowingEclipse()
            );
            
            // Draw sun vector for active satellite (if solar analysis is shown)
            if (ui.isShowingSolar() && activeSatelliteIndex < satellites.size()) {
                Vector3 satPos = RenderUtils::toRaylib(
                    satellites[activeSatelliteIndex].getCurrentState().position
                );
                Vector3D sunDirScaled = sunDirection.normalized() * 5000.0;
                Vector3 sunEnd = RenderUtils::toRaylib(
                    satellites[activeSatelliteIndex].getCurrentState().position + sunDirScaled
                );
                DrawLine3D(satPos, sunEnd, Color{245, 158, 11, 255}); // UITheme::WARNING (yellow)
            }
            
        EndMode3D();
        
        // 2D UI overlay
        ui.draw(
            fonts,
            satellites,
            activeSatelliteIndex,
            currentElements,
            animationSpeed,
            showGrids,
            earthRotation,  // FIXED: Use variable instead of function call
            cameraController.isFollowModeEnabled(),
            GetFPS(),
            sunDirection
        );
        
        EndDrawing();
    }
    
    // Cleanup
    fonts.unload();
    earth.unload();
    CloseWindow();
    
    return 0;
}
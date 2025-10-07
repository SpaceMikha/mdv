#include <iostream>
#include <cmath>
#include <vector>
#include "raylib.h"
#include "Vector3D.h"
#include "StateVector.h"
#include "OrbitPropagator.h"
#include "OrbitalElements.h"
#include "OrbitPresets.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constants
const double MU_EARTH = 398600.4418;  // km^3/s^2
const double EARTH_RADIUS = 6378.137; // km

// Convert km to rendering units
const float SCALE = 0.001f;  // 1 unit = 1000 km

// Camera presets
enum CameraPreset {
    PRESET_DEFAULT,
    PRESET_TOP,
    PRESET_SIDE,
    PRESET_FRONT
};

struct Satellite {
    std::vector<StateVector> orbit;
    size_t currentFrame;
    OrbitPreset preset;
    bool visible;
    
    Satellite(const OrbitPreset& p, const std::vector<StateVector>& o)
        : orbit(o), currentFrame(0), preset(p), visible(true) {}
};

Vector3 toRaylib(const Vector3D& v) {
    return Vector3{
        static_cast<float>(v.x * SCALE),
        static_cast<float>(v.y * SCALE),
        static_cast<float>(v.z * SCALE)
    };
}

void setCameraPreset(Camera3D& camera, CameraPreset preset) {
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
    
    switch (preset) {
        case PRESET_TOP:
            camera.position = Vector3{ 0.0f, 35.0f, 0.01f };
            camera.up = Vector3{ 0.0f, 0.0f, -1.0f };
            break;
        case PRESET_SIDE:
            camera.position = Vector3{ 35.0f, 0.0f, 0.0f };
            camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
            break;
        case PRESET_FRONT:
            camera.position = Vector3{ 0.0f, 0.0f, 35.0f };
            camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
            break;
        case PRESET_DEFAULT:
        default:
            camera.position = Vector3{ 20.0f, 20.0f, 20.0f };
            camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
            break;
    }
}

// Helper function to draw text with custom font
void DrawTextCustom(const char* text, int x, int y, int fontSize, Color color) {
    DrawText(text, x, y, fontSize, color);
}

int main() {
    // Initialize window
    const int screenWidth = 1600;
    const int screenHeight = 900;
    
    InitWindow(screenWidth, screenHeight, "MDV");
    SetTargetFPS(60);
    
    // Enable MSAA antialiasing for smoother rendering
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    
    // Setup 3D camera
    Camera3D camera = { 0 };
    setCameraPreset(camera, PRESET_DEFAULT);
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Create orbit propagator
    OrbitPropagator propagator(MU_EARTH);
    
    // Get all available presets
    std::vector<OrbitPreset> presets = OrbitPresets::getAllPresets(MU_EARTH);
    
    // Create satellites for each preset
    std::vector<Satellite> satellites;
    for (const auto& preset : presets) {
        std::vector<StateVector> orbit = propagator.propagate(
            preset.initialState, 
            preset.period, 
            preset.period / 100.0  // 100 points per orbit
        );
        satellites.push_back(Satellite(preset, orbit));
        
        std::cout << "Orbit created: " << preset.name << " (" << orbit.size() << " points)\n";
    }
    
    // Start with all satellites visible
    for (size_t i = 0; i < satellites.size(); i++) {
        satellites[i].visible = true;
    }
    
    int activeSatellite = 0;  // Currently selected satellite for detailed view
    
    std::cout << "\n=== Controls ===\n";
    std::cout << "TAB:             Cycle active satellite\n";
    std::cout << "H:               Show/Hide elements\n";
    std::cout << "ESC:             Exit\n";
    
    // Animation State
    float animationSpeed = 1.0f;
    float animationAccumulator = 0.0f;

    bool showElements = true;
    bool showMessage = false;
    bool showList = false;
    
    // Main loop
    while (!WindowShouldClose()) {
        
        // Manual camera controls
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouseDelta = GetMouseDelta();
            
            float rotationSpeed = 0.003f;
            
            Vector3 camPos = camera.position;
            Vector3 target = camera.target;
            
            Vector3 direction = { camPos.x - target.x, camPos.y - target.y, camPos.z - target.z };
            float radius = sqrtf(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
            
            float angle = atan2f(direction.z, direction.x);
            angle -= mouseDelta.x * rotationSpeed;
            
            float elevation = asinf(direction.y / radius);
            elevation -= mouseDelta.y * rotationSpeed;
            
            if (elevation > 1.5f) elevation = 1.5f;
            if (elevation < -1.5f) elevation = -1.5f;
            
            camera.position.x = target.x + radius * cosf(elevation) * cosf(angle);
            camera.position.y = target.y + radius * sinf(elevation);
            camera.position.z = target.z + radius * cosf(elevation) * sinf(angle);
        }
        
        // Mouse wheel zoom
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector3 direction = { camera.position.x - camera.target.x,
                                camera.position.y - camera.target.y,
                                camera.position.z - camera.target.z };
            float distance = sqrtf(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
            
            float zoomSpeed = distance * 0.1f;
            distance -= wheel * zoomSpeed;
            
            if (distance < 10.0f) distance = 10.0f;
            if (distance > 200.0f) distance = 200.0f;
            
            float currentDist = sqrtf(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
            direction.x = direction.x / currentDist * distance;
            direction.y = direction.y / currentDist * distance;
            direction.z = direction.z / currentDist * distance;
            
            camera.position.x = camera.target.x + direction.x;
            camera.position.y = camera.target.y + direction.y;
            camera.position.z = camera.target.z + direction.z;
        }
        
        // Update animation with accumulator for fractional speeds
        if (animationSpeed > 0.01f) {
            animationAccumulator += animationSpeed;
            size_t framesToAdvance = static_cast<size_t>(animationAccumulator);
            animationAccumulator -= framesToAdvance;  // Keep the fractional part
            
            for (auto& sat : satellites) {
                if (sat.visible) {
                    sat.currentFrame = (sat.currentFrame + framesToAdvance) % sat.orbit.size();
                }
            }
        }
        
        // Speed controls
        if (IsKeyPressed(KEY_SPACE)) {
            animationSpeed = (animationSpeed > 0.01f) ? 0.0f : 1.0f;
        }
        if (IsKeyPressed(KEY_UP)) animationSpeed *= 2.0f;
        if (IsKeyPressed(KEY_DOWN)) animationSpeed *= 0.5f;
        if (animationSpeed > 10.0f) animationSpeed = 10.0f;
        if (animationSpeed < 0.05f && animationSpeed > 0.0f) animationSpeed = 0.05f;

        // Show commands menu
        if (IsKeyPressed(KEY_M)) showMessage = !showMessage;

        // Show panel list
        if(IsKeyPressed(KEY_C)) showList = !showList;
        
        // Camera presets
        if (IsKeyPressed(KEY_ONE)) setCameraPreset(camera, PRESET_DEFAULT);
        if (IsKeyPressed(KEY_TWO)) setCameraPreset(camera, PRESET_TOP);
        if (IsKeyPressed(KEY_THREE)) setCameraPreset(camera, PRESET_SIDE);
        if (IsKeyPressed(KEY_FOUR)) setCameraPreset(camera, PRESET_FRONT);
        
        // Toggle elements display
        if (IsKeyPressed(KEY_H)) showElements = !showElements;

        
        // Toggle satellite visibility (Q/W/E/R/T/Y/U/I/O/P for satellites 0-9)
        if (IsKeyPressed(KEY_Q) && satellites.size() > 0) satellites[0].visible = !satellites[0].visible;
        if (IsKeyPressed(KEY_W) && satellites.size() > 1) satellites[1].visible = !satellites[1].visible;
        if (IsKeyPressed(KEY_E) && satellites.size() > 2) satellites[2].visible = !satellites[2].visible;
        if (IsKeyPressed(KEY_R) && satellites.size() > 3) satellites[3].visible = !satellites[3].visible;
        if (IsKeyPressed(KEY_T) && satellites.size() > 4) satellites[4].visible = !satellites[4].visible;
        if (IsKeyPressed(KEY_Y) && satellites.size() > 5) satellites[5].visible = !satellites[5].visible;
        if (IsKeyPressed(KEY_U) && satellites.size() > 6) satellites[6].visible = !satellites[6].visible;
        if (IsKeyPressed(KEY_I) && satellites.size() > 7) satellites[7].visible = !satellites[7].visible;
        if (IsKeyPressed(KEY_O) && satellites.size() > 8) satellites[8].visible = !satellites[8].visible;
        if (IsKeyPressed(KEY_P) && satellites.size() > 9) satellites[9].visible = !satellites[9].visible;
        
        // Cycle active satellite
        if (IsKeyPressed(KEY_TAB)) {
            do {
                activeSatellite = (activeSatellite + 1) % satellites.size();
            } while (!satellites[activeSatellite].visible && activeSatellite != 0);
        }
        
        // Get current orbital elements for active satellite
        OrbitalElements currentElements = OrbitalElements::fromStateVector(
            satellites[activeSatellite].orbit[satellites[activeSatellite].currentFrame], 
            MU_EARTH
        );
        
        // Draw
        BeginDrawing();
        ClearBackground(Color{ 31, 7, 41 });
        
        BeginMode3D(camera);
        
            // Draw Earth
            DrawSphere(Vector3{0, 0, 0}, EARTH_RADIUS * SCALE, BLUE);
            DrawSphereWires(Vector3{0, 0, 0}, EARTH_RADIUS * SCALE, 16, 16, DARKBLUE);
            
            // Draw coordinate axes
            float axisLength = 10.0f;
            DrawLine3D(Vector3{0, 0, 0}, Vector3{axisLength, 0, 0}, RED);
            DrawLine3D(Vector3{0, 0, 0}, Vector3{0, axisLength, 0}, GREEN);
            DrawLine3D(Vector3{0, 0, 0}, Vector3{0, 0, axisLength}, SKYBLUE);
            
            // Draw all visible satellites and orbits
            for (size_t s = 0; s < satellites.size(); s++) {
                if (!satellites[s].visible) continue;
                
                auto& sat = satellites[s];
                Color orbitColor = sat.preset.color;
                Color dimmedColor = Fade(orbitColor, 0.3f);
                bool isActive = (s == activeSatellite);
                
                // Draw orbit trajectory with variable opacity
                Color lineColor = isActive ? orbitColor : Fade(orbitColor, 0.5f);
                for (size_t i = 1; i < sat.orbit.size(); i++) {
                    Vector3 p1 = toRaylib(sat.orbit[i-1].position);
                    Vector3 p2 = toRaylib(sat.orbit[i].position);
                    DrawLine3D(p1, p2, lineColor);
                }
                
                // Draw spacecraft
                Vector3 scPos = toRaylib(sat.orbit[sat.currentFrame].position);
                float satSize = isActive ? 0.4f : 0.25f;
                DrawSphere(scPos, satSize, orbitColor);
                
                // Draw velocity vector for active satellite
                if (isActive) {
                    Vector3D velScaled = sat.orbit[sat.currentFrame].velocity.normalized() * 2000.0;
                    Vector3 velEnd = toRaylib(sat.orbit[sat.currentFrame].position + velScaled);
                    DrawLine3D(scPos, velEnd, GREEN);
                }
                
                // Draw periapsis and apoapsis markers for elliptical orbits (active only)
                if (isActive) {
                    OrbitalElements elements = OrbitalElements::fromStateVector(sat.orbit[0], MU_EARTH);
                    if (elements.eccentricity > 0.01) {
                        // Periapsis marker (closest point) - Orange sphere
                        size_t periIdx = 0;
                        DrawSphere(toRaylib(sat.orbit[periIdx].position), 0.3f, ORANGE);
                        
                        // Apoapsis marker (farthest point) - Purple sphere
                        size_t apoIdx = sat.orbit.size() / 2;
                        DrawSphere(toRaylib(sat.orbit[apoIdx].position), 0.3f, PURPLE);
                        
                        // Draw apse line
                        DrawLine3D(
                            toRaylib(sat.orbit[periIdx].position),
                            toRaylib(sat.orbit[apoIdx].position),
                            Fade(WHITE, 0.3f)
                        );
                    }
                }
                
                // Draw small trail behind satellite (last 10 positions)
                if (sat.currentFrame > 25) {
                    for (size_t i = sat.currentFrame - 25; i < sat.currentFrame; i++) {
                        float alpha = (float)(i - (sat.currentFrame - 10)) / 10.0f;
                        Vector3 trailPos = toRaylib(sat.orbit[i].position);
                        DrawSphere(trailPos, 0.1f, Fade(orbitColor, alpha * 0.5f));
                    }
                }
            }
            
        EndMode3D();
        
        // Draw UI - Title
        DrawText("M.D.V - Under Development", 650, 10, 24, RED);
          
        // Status bar
        int yPos = 45;
        yPos += 825;
        DrawText(TextFormat("Speed: %.1fx | Active: %s | FPS: %d", 
                 animationSpeed, 
                 satellites[activeSatellite].preset.name.c_str(),
                 GetFPS()), 
                 10, yPos, 16, satellites[activeSatellite].preset.color);
        
        

        // Satellite list panel
        if (showList) {
        
        int listX = 10;
        int listY = 120;
        int listW = 280;
        int listH = 30 + satellites.size() * 25;
        
        DrawRectangle(listX, listY, listW, listH, Fade(BLACK, 0.8f));
        DrawRectangleLines(listX, listY, listW, listH, SKYBLUE);
        
        listY += 10;
        listX += 10;
        
        DrawText("SATELLITES", listX, listY, 16, SKYBLUE);
        listY += 25;
        
        for (size_t i = 0; i < satellites.size(); i++) {
            Color textColor = satellites[i].visible ? satellites[i].preset.color : GRAY;
            const char* activeMarker = (i == activeSatellite) ? "> " : "  ";
            DrawText(TextFormat("%s%s", activeMarker, satellites[i].preset.name.c_str()), 
                     listX, listY, 14, textColor);
            listY += 25;
        }
        }
        

        // Show commands menu (at the top)
        if (showMessage) {
            int yPos = 45;

           // DrawText("Commands: C: Show Satellites List", 
           // 10, yPos, 14, GRAY);

            int panelX = screenWidth - 1590;
            int panelY = 80;
            int panelW = 320;
            int panelH = 220;
            
            DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.8f));
            DrawRectangleLines(panelX, panelY, panelW, panelH, satellites[activeSatellite].preset.color);
            
            panelY += 10;
            panelX += 10;
            
            DrawText("Commands", panelX + 80, panelY, 25, RED);
            panelY += 25;
            
            DrawText("Commands for Simulation", 
                     panelX, panelY + 15, 11, LIGHTGRAY);
            panelY += 30;
            
            DrawText(TextFormat("Type: %s", currentElements.orbitType().c_str()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            

        }

        // Orbital elements panel (right side)
        if (showElements) {
            int panelX = screenWidth - 320;
            int panelY = 100;
            int panelW = 310;
            int panelH = 420;
            
            DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.8f));
            DrawRectangleLines(panelX, panelY, panelW, panelH, satellites[activeSatellite].preset.color);
            
            panelY += 10;
            panelX += 10;
            
            DrawText("ORBITAL ELEMENTS", panelX, panelY, 18, satellites[activeSatellite].preset.color);
            panelY += 25;
            
            DrawText(satellites[activeSatellite].preset.description.c_str(), 
                     panelX, panelY, 11, LIGHTGRAY);
            panelY += 30;
            
            DrawText(TextFormat("Type: %s", currentElements.orbitType().c_str()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText("Semi-major axis (a)", panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f km", currentElements.semiMajorAxis), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText("Eccentricity (e)", panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.6f", currentElements.eccentricity), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText("Inclination (i)", panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f deg", currentElements.inclinationDeg()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText("RAAN (Omega)", panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f deg", currentElements.raanDeg()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText("Arg. Periapsis (omega)", panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f deg", currentElements.argumentOfPeriapsisDeg()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText("True Anomaly (nu)", panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f deg", currentElements.trueAnomalyDeg()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText(TextFormat("Period: %.2f min", currentElements.period / 60.0), 
                     panelX, panelY, 14, GREEN);
            panelY += 20;
            
            DrawText(TextFormat("Altitude: %.1f km", 
                     satellites[activeSatellite].orbit[satellites[activeSatellite].currentFrame].altitude(EARTH_RADIUS)), 
                     panelX, panelY, 14, GREEN);
        }
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
#include <iostream>
#include <cmath>
#include <vector>
#include "raylib.h"
#include "Vector3D.h"
#include "StateVector.h"
#include "OrbitPropagator.h"
#include "OrbitalElements.h"

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
            camera.position = Vector3{ 0.0f, 35.0f, 0.01f };  // Slightly offset to avoid gimbal lock
            camera.up = Vector3{ 0.0f, 0.0f, -1.0f };  // Z-axis points "up" from top view
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

int main() {
    // Initialize window
    const int screenWidth = 1600;
    const int screenHeight = 900;
    
    InitWindow(screenWidth, screenHeight, "MDV - By: Mikhael da Silva");
    SetTargetFPS(60);
    
    // Setup 3D camera
    Camera3D camera = { 0 };
    setCameraPreset(camera, PRESET_DEFAULT);
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Create ISS orbit
    double altitude = 400.0;  // km
    double r = EARTH_RADIUS + altitude;
    double v_circular = std::sqrt(MU_EARTH / r);
    
    Vector3D pos(r, 0.0, 0.0);
    Vector3D vel(0.0, v_circular, 0.0);
    StateVector initialState(pos, vel, 0.0);
    
    // Calculate initial orbital elements
    OrbitalElements elements = OrbitalElements::fromStateVector(initialState, MU_EARTH);
    elements.print();
    
    // Propagate orbit
    OrbitPropagator propagator(MU_EARTH);
    double period = 2.0 * M_PI * std::sqrt(r*r*r / MU_EARTH);
    std::vector<StateVector> orbit = propagator.propagate(initialState, period, 30.0);
    
    std::cout << "\nOrbit propagated: " << orbit.size() << " points\n";
    std::cout << "\n=== Controls ===\n";
    std::cout << "Right Mouse:     Rotate camera\n";
    std::cout << "Mouse Wheel:     Zoom\n";
    std::cout << "SPACE:           Pause/Play\n";
    std::cout << "UP/DOWN:         Speed control\n";
    std::cout << "1/2/3/4:         Camera presets\n";
    std::cout << "E:               Show/Hide elements\n";
    std::cout << "ESC:             Exit\n";
    
    // Animation State
    size_t currentFrame = 0;
    float animationSpeed = 1.0f;
    bool showElements = true;
    
    // Camera control mode
    bool freeCameraMode = true;
    
    // Main loop
    while (!WindowShouldClose()) {
        
        // Manual camera controls
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouseDelta = GetMouseDelta();
            
            // Rotate camera around target
            float rotationSpeed = 0.003f;
            
            // Get camera vectors
            Vector3 camPos = camera.position;
            Vector3 target = camera.target;
            
            // Calculate spherical coordinates
            Vector3 direction = { camPos.x - target.x, camPos.y - target.y, camPos.z - target.z };
            float radius = sqrtf(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
            
            // Horizontal rotation (around Y axis)
            float angle = atan2f(direction.z, direction.x);
            angle -= mouseDelta.x * rotationSpeed;
            
            // Vertical rotation
            float elevation = asinf(direction.y / radius);
            elevation -= mouseDelta.y * rotationSpeed;
            
            // Clamp elevation to avoid flipping
            if (elevation > 1.5f) elevation = 1.5f;
            if (elevation < -1.5f) elevation = -1.5f;
            
            // Convert back to Cartesian
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
            
            // Zoom in/out
            float zoomSpeed = distance * 0.1f;
            distance -= wheel * zoomSpeed;
            
            // Clamp distance
            if (distance < 10.0f) distance = 10.0f;
            if (distance > 100.0f) distance = 100.0f;
            
            // Normalize and scale
            float currentDist = sqrtf(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
            direction.x = direction.x / currentDist * distance;
            direction.y = direction.y / currentDist * distance;
            direction.z = direction.z / currentDist * distance;
            
            camera.position.x = camera.target.x + direction.x;
            camera.position.y = camera.target.y + direction.y;
            camera.position.z = camera.target.z + direction.z;
        }
        
        // Update animation
        if (animationSpeed > 0.0f) {
            currentFrame = (currentFrame + static_cast<size_t>(animationSpeed)) % orbit.size();
        }
        
        // Speed controls
        if (IsKeyPressed(KEY_SPACE)) {
            animationSpeed = (animationSpeed > 0.01f) ? 0.0f : 1.0f;
        }
        if (IsKeyPressed(KEY_UP)) animationSpeed *= 2.0f;
        if (IsKeyPressed(KEY_DOWN)) animationSpeed *= 0.5f;
        if (animationSpeed > 10.0f) animationSpeed = 10.0f;
        if (animationSpeed < 0.1f && animationSpeed > 0.0f) animationSpeed = 0.1f;
        
        // Camera presets
        if (IsKeyPressed(KEY_ONE)) setCameraPreset(camera, PRESET_DEFAULT);
        if (IsKeyPressed(KEY_TWO)) setCameraPreset(camera, PRESET_TOP);
        if (IsKeyPressed(KEY_THREE)) setCameraPreset(camera, PRESET_SIDE);
        if (IsKeyPressed(KEY_FOUR)) setCameraPreset(camera, PRESET_FRONT);
        
        // Toggle elements display
        if (IsKeyPressed(KEY_E)) showElements = !showElements;
        
        // Recalculate orbital elements for current state
        OrbitalElements currentElements = OrbitalElements::fromStateVector(
            orbit[currentFrame], MU_EARTH
        );
        
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        
        BeginMode3D(camera);
        
            // Draw Earth
            DrawSphere(Vector3{0, 0, 0}, EARTH_RADIUS * SCALE, BLUE);
            DrawSphereWires(Vector3{0, 0, 0}, EARTH_RADIUS * SCALE, 16, 16, DARKBLUE);
            
            // Draw coordinate axes
            float axisLength = 10.0f;
            DrawLine3D(Vector3{0, 0, 0}, Vector3{axisLength, 0, 0}, RED);
            DrawLine3D(Vector3{0, 0, 0}, Vector3{0, axisLength, 0}, GREEN);
            DrawLine3D(Vector3{0, 0, 0}, Vector3{0, 0, axisLength}, SKYBLUE);
            
            // Draw orbit trajectory
            for (size_t i = 1; i < orbit.size(); i++) {
                Vector3 p1 = toRaylib(orbit[i-1].position);
                Vector3 p2 = toRaylib(orbit[i].position);
                DrawLine3D(p1, p2, YELLOW);
            }
            
            // Draw animated spacecraft
            Vector3 scPos = toRaylib(orbit[currentFrame].position);
            DrawSphere(scPos, 0.3f, RED);
            
            // Draw velocity vector
            Vector3D velScaled = orbit[currentFrame].velocity.normalized() * 1000.0;
            Vector3 velEnd = toRaylib(orbit[currentFrame].position + velScaled);
            DrawLine3D(scPos, velEnd, GREEN);
            
            // Draw periapsis and apoapsis markers
            if (currentElements.eccentricity > 0.01) {
                // Periapsis (closest point)
                DrawSphere(toRaylib(orbit[0].position), 0.2f, ORANGE);
                
                // Find apoapsis (opposite side)
                size_t apoIdx = orbit.size() / 2;
                DrawSphere(toRaylib(orbit[apoIdx].position), 0.2f, PURPLE);
            }
            
        EndMode3D();
        
        // Draw UI - Title
        DrawText("Mission Design Tool", 10, 10, 24, WHITE);
        
        // Controls info
        int yPos = 45;
        DrawText("Controls: RMB: Rotate | Wheel: Zoom | SPACE: Pause | UP/DOWN: Speed | 1-4: Camera | E: Elements", 
                 10, yPos, 14, LIGHTGRAY);
        
        // Status bar
        yPos += 30;
        DrawText(TextFormat("Speed: %.1fx  |  Time: %.1f min  |  FPS: %d", 
                 animationSpeed, 
                 orbit[currentFrame].time/60.0,
                 GetFPS()), 
                 10, yPos, 16, YELLOW);
        
        // Orbital elements panel
        if (showElements) {
            int panelX = screenWidth - 320;
            int panelY = 100;
            int panelW = 310;
            int panelH = 400;
            
            // Semi-transparent background
            DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.8f));
            DrawRectangleLines(panelX, panelY, panelW, panelH, YELLOW);
            
            panelY += 10;
            panelX += 10;
            
            DrawText("ORBITAL ELEMENTS", panelX, panelY, 18, YELLOW);
            panelY += 30;
            
            DrawText(TextFormat("Type: %s", currentElements.orbitType().c_str()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText(TextFormat("Semi-major axis (a)"), panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f km", currentElements.semiMajorAxis), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText(TextFormat("Eccentricity (e)"), panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.6f", currentElements.eccentricity), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText(TextFormat("Inclination (i)"), panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f deg", currentElements.inclinationDeg()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText(TextFormat("RAAN (Omega)"), panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f deg", currentElements.raanDeg()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText(TextFormat("Arg. Periapsis (omega)"), panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f deg", currentElements.argumentOfPeriapsisDeg()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText(TextFormat("True Anomaly (nu)"), panelX, panelY, 14, LIGHTGRAY);
            panelY += 18;
            DrawText(TextFormat("  %.2f deg", currentElements.trueAnomalyDeg()), 
                     panelX, panelY, 14, WHITE);
            panelY += 25;
            
            DrawText(TextFormat("Period: %.2f min", currentElements.period / 60.0), 
                     panelX, panelY, 14, GREEN);
        }
        
        // State vector panel (left side)
        int stateX = 10;
        int stateY = 100;
        int stateW = 280;
        int stateH = 180;
        
        DrawRectangle(stateX, stateY, stateW, stateH, Fade(BLACK, 0.8f));
        DrawRectangleLines(stateX, stateY, stateW, stateH, SKYBLUE);
        
        stateY += 10;
        stateX += 10;
        
        DrawText("STATE VECTOR", stateX, stateY, 18, SKYBLUE);
        stateY += 30;
        
        Vector3D pos_curr = orbit[currentFrame].position;
        Vector3D vel_curr = orbit[currentFrame].velocity;
        
        DrawText(TextFormat("Position (km):"), stateX, stateY, 14, LIGHTGRAY);
        stateY += 20;
        DrawText(TextFormat("  X: %8.2f", pos_curr.x), stateX, stateY, 13, WHITE);
        stateY += 18;
        DrawText(TextFormat("  Y: %8.2f", pos_curr.y), stateX, stateY, 13, WHITE);
        stateY += 18;
        DrawText(TextFormat("  Z: %8.2f", pos_curr.z), stateX, stateY, 13, WHITE);
        stateY += 25;
        
        DrawText(TextFormat("Velocity (km/s):"), stateX, stateY, 14, LIGHTGRAY);
        stateY += 20;
        DrawText(TextFormat("  %.4f", vel_curr.magnitude()), stateX, stateY, 13, GREEN);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
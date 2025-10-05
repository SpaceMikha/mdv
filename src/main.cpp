#include <iostream>
#include <cmath>
#include <vector>
#include "raylib.h"
#include "Vector3D.h"
#include "StateVector.h"
#include "OrbitPropagator.h"

//Defining PI 
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constants
const double MU_EARTH = 398600.4418;  // km^3/s^2
const double EARTH_RADIUS = 6378.137; // km

// Convert km to rendering units (scale down for visualization)
const float SCALE = 0.001f;  // 1 unit = 1000 km

Vector3 toRaylib(const Vector3D& v) {
    return Vector3{
        static_cast<float>(v.x * SCALE),
        static_cast<float>(v.y * SCALE),
        static_cast<float>(v.z * SCALE)
    };
}

int main() {
    // Initialize window
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    
    InitWindow(screenWidth, screenHeight, "Mission Design Tool - Phase 2");
    SetTargetFPS(60);
    
    // Setup 3D camera
    Camera3D camera = { 0 };
    camera.position = Vector3{ 20.0f, 20.0f, 20.0f };
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Create ISS orbit
    double altitude = 400.0;  // km
    double r = EARTH_RADIUS + altitude;
    double v_circular = std::pow(MU_EARTH / r, 0.5);
    
    Vector3D pos(r, 0.0, 0.0);
    Vector3D vel(0.0, v_circular, 0.0);
    StateVector initialState(pos, vel, 0.0);
    
    // Propagate orbit
    OrbitPropagator propagator(MU_EARTH);
    double period = 2.0 * M_PI * std::pow(r*r*r / MU_EARTH, 0.5);
    std::vector<StateVector> orbit = propagator.propagate(initialState, period, 30.0);
    
    std::cout << "Orbit propagated: " << orbit.size() << " points\n";
    std::cout << "Controls:\n";
    std::cout << "  - Right mouse button: Rotate camera\n";
    std::cout << "  - Mouse wheel: Zoom\n";
    std::cout << "  - ESC: Exit\n";
    
     // Animation State
    size_t currentFrame = 0;
    float animationSpeed = 1.0f;

    // Main loop
    while (!WindowShouldClose()) {
        
        // Update camera
        UpdateCamera(&camera, CAMERA_ORBITAL);
        
        // Update animation
        currentFrame = (currentFrame + static_cast<size_t>(animationSpeed)) % orbit.size();
        
        // Speed controls
        if (IsKeyPressed(KEY_SPACE)) animationSpeed = (animationSpeed > 0.1f) ? 0.0f : 1.0f;  // Pause/Play
        if (IsKeyPressed(KEY_UP)) animationSpeed *= 2.0f;    // Speed up
        if (IsKeyPressed(KEY_DOWN)) animationSpeed *= 0.5f;  // Slow down
        if (animationSpeed > 10.0f) animationSpeed = 10.0f;  // Max speed
        if (animationSpeed < 0.1f && animationSpeed > 0.0f) animationSpeed = 0.1f;  // Min speed
        
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        
        BeginMode3D(camera);
        
            // Draw Earth
            DrawSphere(Vector3{0, 0, 0}, EARTH_RADIUS * SCALE, BLUE);
            DrawSphereWires(Vector3{0, 0, 0}, EARTH_RADIUS * SCALE, 16, 16, DARKBLUE);
            
            // Draw coordinate axes
            DrawLine3D(Vector3{0, 0, 0}, Vector3{10, 0, 0}, RED);
            DrawLine3D(Vector3{0, 0, 0}, Vector3{0, 10, 0}, GREEN);
            DrawLine3D(Vector3{0, 0, 0}, Vector3{0, 0, 10}, BLUE);
            
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
            
        EndMode3D();
        
        // Draw UI
        DrawText("Mission Design Tool - ISS Orbit Visualization", 10, 10, 20, WHITE);
        DrawText("Right Mouse: Rotate | Wheel: Zoom | SPACE: Pause | UP/DOWN: Speed", 10, 40, 16, LIGHTGRAY);
        DrawText(TextFormat("Speed: %.1fx", animationSpeed), 10, 60, 16, YELLOW);
        DrawText(TextFormat("Time: %.1f min", orbit[currentFrame].time/60.0), 10, 80, 16, YELLOW);
        DrawText(TextFormat("Altitude: %.1f km", orbit[currentFrame].altitude(EARTH_RADIUS)), 10, 100, 16, YELLOW);
        DrawText(TextFormat("Velocity: %.3f km/s", orbit[currentFrame].speed()), 10, 120, 16, YELLOW);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 680, 16, LIGHTGRAY);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
#include <iostream>
#include <cmath>
#include <vector>
#include "raylib.h"
#include "rlgl.h"
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

Vector3 toRaylib(const Vector3D& v);

// Camera presets
enum CameraPreset {
    PRESET_DEFAULT,
    PRESET_TOP,
    PRESET_SIDE,
    PRESET_FRONT
};

// Font system
struct FontSystem {
    Font regular;
    Font bold;
    bool loaded;
    
    FontSystem() : loaded(false) {}
    
    void load() {
        std::cout << "Loading fonts...\n";
        
        regular = LoadFontEx("../assets/fonts/Roboto-Regular.ttf", 72, 0, 250);
        bold = LoadFontEx("../assets/fonts/Roboto-Bold.ttf", 72, 0, 250);
        
        if (regular.texture.id == 0) {
            std::cout << "  Roboto not found, trying Segoe UI...\n";
            regular = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 72, 0, 250);
            if (regular.texture.id == 0) {
                std::cout << "  Using default font\n";
                regular = GetFontDefault();
            } else {
                std::cout << "  Segoe UI loaded!\n";
            }
        } else {
            std::cout << "  Roboto-Regular.ttf loaded!\n";
        }
        
        if (bold.texture.id == 0) {
            std::cout << "  Roboto Bold not found, trying Segoe UI Bold...\n";
            bold = LoadFontEx("C:/Windows/Fonts/segoeuib.ttf", 72, 0, 250);
            if (bold.texture.id == 0) {
                std::cout << "  Using default font\n";
                bold = GetFontDefault();
            } else {
                std::cout << "  Segoe UI Bold loaded!\n";
            }
        } else {
            std::cout << "  Roboto-Bold.ttf loaded!\n";
        }
        
        if (regular.texture.id != 1) {
            SetTextureFilter(regular.texture, TEXTURE_FILTER_BILINEAR);
        }
        if (bold.texture.id != 1) {
            SetTextureFilter(bold.texture, TEXTURE_FILTER_BILINEAR);
        }
        
        loaded = true;
    }
    
    void unload() {
        if (loaded) {
            if (regular.texture.id != 1) UnloadFont(regular);
            if (bold.texture.id != 1) UnloadFont(bold);
            loaded = false;
        }
    }
};

// Earth rendering system
struct EarthSystem {
    Model earthModel;
    Texture2D earthTexture;
    bool textureLoaded;
    float rotationAngle;
    
    EarthSystem() : textureLoaded(false), rotationAngle(0.0f) {}
    
    void load() {
        std::cout << "Loading Earth...\n";
        
        // Create a sphere mesh for Earth with proper aspect ratio
        // Use more rings than slices to prevent squashing
        Mesh sphereMesh = GenMeshSphere(EARTH_RADIUS * SCALE, 64, 64);
        earthModel = LoadModelFromMesh(sphereMesh);
        
        // Try to load texture
        earthTexture = LoadTexture("../assets/textures/planet.png");
        if (earthTexture.id == 0) {
            earthTexture = LoadTexture("../assets/textures/planet.jpg");
        }
        
        if (earthTexture.id != 0) {
            std::cout << "  Earth texture loaded!\n";
            
            // Set texture wrapping to repeat
            SetTextureWrap(earthTexture, TEXTURE_WRAP_REPEAT);
            SetTextureFilter(earthTexture, TEXTURE_FILTER_BILINEAR);
            
            SetMaterialTexture(&earthModel.materials[0], MATERIAL_MAP_DIFFUSE, earthTexture);
            textureLoaded = true;
        } else {
            std::cout << "  No texture found, using default blue sphere\n";
            textureLoaded = false;
        }
    }
    
    void update(float deltaTime) {
        // Rotate Earth slowly (one full rotation every 60 seconds)
        rotationAngle += deltaTime * 6.0f;  // 6 degrees per second = 360 degrees in 60 seconds
        if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
    }
    
    void draw() {
        if (textureLoaded) {
            // Draw with proper orientation corrections
            rlPushMatrix();
                // First: Flip the texture right-side up (rotate 180° around X)
                rlRotatef(180.0f, 1, 0, 0);
                
                // Second: Rotate 180° to put Americas on the right side
                rlRotatef(180.0f, 0, 1, 0);
                
                // Third: Apply Earth's 23.5° axial tilt
                rlRotatef(-23.5f, 1, 0, 0);
                
                // Fourth: Apply day/night rotation
                rlRotatef(rotationAngle, 0, 1, 0);
                
                DrawModel(earthModel, Vector3{0, 0, 0}, 1.0f, WHITE);
            rlPopMatrix();
        } else {
            // Fallback: Draw plain blue sphere with tilt
            rlPushMatrix();
                rlRotatef(23.5f, 1, 0, 0);  // Apply 23.5° tilt
                rlRotatef(rotationAngle, 0, 1, 0);  // Rotate around tilted Y axis
                DrawSphere(Vector3{0, 0, 0}, EARTH_RADIUS * SCALE, BLUE);
            rlPopMatrix();
        }
    }
    
    void unload() {
        if (textureLoaded) {
            UnloadTexture(earthTexture);
        }
        UnloadModel(earthModel);
    }
};

// Helper functions for text rendering
void DrawTextPro(const FontSystem& fonts, const char* text, float x, float y, float fontSize, Color color, bool bold = false) {
    Font font = bold ? fonts.bold : fonts.regular;
    Vector2 position = { x, y };
    float spacing = 0.0f;
    DrawTextEx(font, text, position, fontSize, spacing, color);
}

void MeasureTextPro(const FontSystem& fonts, const char* text, float fontSize, float* width, float* height) {
    Vector2 size = MeasureTextEx(fonts.regular, text, fontSize, 0.0f);
    if (width) *width = size.x;
    if (height) *height = size.y;
}

struct OrbitStatistics {
    double periapsisAlt;
    double apoapsisAlt;
    double periapsisVel;
    double apoapsisVel;
    double meanAltitude;
    std::string orbitFamily;
    Color familyColor;
};

struct Satellite {
    std::vector<StateVector> orbit;
    size_t currentFrame;
    OrbitPreset preset;
    bool visible;
    OrbitStatistics stats;
    
    Satellite(const OrbitPreset& p, const std::vector<StateVector>& o)
        : orbit(o), currentFrame(0), preset(p), visible(true) {
        calculateStatistics();
    }
    
    void calculateStatistics() {
        if (orbit.empty()) return;
        
        double minR = 1e10, maxR = 0;
        size_t periIdx = 0, apoIdx = 0;
        
        for (size_t i = 0; i < orbit.size(); i++) {
            double r = orbit[i].position.magnitude();
            if (r < minR) {
                minR = r;
                periIdx = i;
            }
            if (r > maxR) {
                maxR = r;
                apoIdx = i;
            }
        }
        
        stats.periapsisAlt = minR - EARTH_RADIUS;
        stats.apoapsisAlt = maxR - EARTH_RADIUS;
        stats.periapsisVel = orbit[periIdx].velocity.magnitude();
        stats.apoapsisVel = orbit[apoIdx].velocity.magnitude();
        stats.meanAltitude = (stats.periapsisAlt + stats.apoapsisAlt) / 2.0;
        
        if (stats.meanAltitude < 2000.0) {
            stats.orbitFamily = "LEO";
            stats.familyColor = Color{100, 200, 255, 255};
        } else if (stats.meanAltitude < 35000.0) {
            stats.orbitFamily = "MEO";
            stats.familyColor = Color{100, 255, 100, 255};
        } else if (stats.apoapsisAlt > 35000.0 && stats.periapsisAlt < 35000.0) {
            stats.orbitFamily = "HEO";
            stats.familyColor = Color{255, 150, 100, 255};
        } else {
            stats.orbitFamily = "GEO";
            stats.familyColor = Color{255, 100, 255, 255};
        }
    }
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

// Draw infinite equatorial plane grid
void drawEquatorialGrid() {
    float spacing = 5.0f;
    int numLines = 20;
    
    for (int i = -numLines; i <= numLines; i++) {
        float z = i * spacing;
        float distFromCenter = fabs((float)i) / (float)numLines;
        float alpha = (1.0f - distFromCenter * distFromCenter) * 0.4f;
        Color lineColor = Fade(GRAY, alpha);
        
        if (i == 0) {
            lineColor = Fade(SKYBLUE, 0.3f);
        }
        
        DrawLine3D(
            Vector3{-numLines * spacing, 0.01f, z},
            Vector3{numLines * spacing, 0.01f, z},
            lineColor
        );
    }
    
    for (int i = -numLines; i <= numLines; i++) {
        float x = i * spacing;
        float distFromCenter = fabs((float)i) / (float)numLines;
        float alpha = (1.0f - distFromCenter * distFromCenter) * 0.4f;
        Color lineColor = Fade(GRAY, alpha);
        
        if (i == 0) {
            lineColor = Fade(RED, 0.3f);
        }
        
        DrawLine3D(
            Vector3{x, 0.01f, -numLines * spacing},
            Vector3{x, 0.01f, numLines * spacing},
            lineColor
        );
    }
}

// Draw reference altitude circles
void drawReferenceCircles() {
    const int segments = 64;
    
    float leoRadius = (EARTH_RADIUS + 2000.0) * SCALE;
    Color leoColor = Fade(Color{100, 200, 255, 255}, 0.3f);
    DrawCircle3D(Vector3{0, 0, 0}, leoRadius, Vector3{1, 0, 0}, 90.0f, leoColor);
    
    float geoRadius = (EARTH_RADIUS + 35786.0) * SCALE;
    Color geoColor = Fade(Color{255, 100, 255, 255}, 0.3f);
    DrawCircle3D(Vector3{0, 0, 0}, geoRadius, Vector3{1, 0, 0}, 90.0f, geoColor);
}

// Draw orbital plane
void drawOrbitalPlane(const std::vector<StateVector>& orbit, Color color) {
    if (orbit.size() < 3) return;
    
    Vector3D r = orbit[0].position;
    Vector3D v = orbit[0].velocity;
    Vector3D h = r.cross(v);
    h = h.normalized();
    
    double maxR = 0;
    for (const auto& state : orbit) {
        double r = state.position.magnitude();
        if (r > maxR) maxR = r;
    }
    
    float planeSize = maxR * SCALE * 1.2f;
    
    int gridLines = 10;
    float step = planeSize * 2.0f / gridLines;
    
    Vector3D up(0, 1, 0);
    Vector3D right = h.cross(up).normalized();
    Vector3D forward = right.cross(h).normalized();
    
    Color lineColor = Fade(color, 0.25f);
    
    for (int i = -gridLines/2; i <= gridLines/2; i++) {
        float offset = i * step;
        
        Vector3D start1 = right * (-planeSize) + forward * offset;
        Vector3D end1 = right * planeSize + forward * offset;
        DrawLine3D(toRaylib(start1), toRaylib(end1), lineColor);
        
        Vector3D start2 = forward * (-planeSize) + right * offset;
        Vector3D end2 = forward * planeSize + right * offset;
        DrawLine3D(toRaylib(start2), toRaylib(end2), lineColor);
    }
}

void drawKeyboardLegend(const FontSystem& fonts, int screenWidth, int screenHeight) {
    int panelW = 650;
    int panelH = 600;
    int panelX = (screenWidth - panelW) / 2;
    int panelY = (screenHeight - panelH) / 2;
    
    DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.95f));
    DrawRectangleLines(panelX, panelY, panelW, panelH, SKYBLUE);
    
    int x = panelX + 20;
    int y = panelY + 15;
    
    DrawTextPro(fonts, "KEYBOARD SHORTCUTS", x + 160, y, 24, SKYBLUE, true);
    y += 45;
    
    int col1X = x;
    int col2X = x + 320;
    
    DrawTextPro(fonts, "SIMULATION CONTROL", col1X, y, 16, YELLOW, true);
    y += 28;
    DrawTextPro(fonts, "SPACE", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Pause/Resume", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "UP/DOWN", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Speed Up/Down", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "R", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Toggle Earth Rotation", col1X + 90, y, 14, LIGHTGRAY);
    y += 32;
    
    DrawTextPro(fonts, "CAMERA CONTROL", col1X, y, 16, YELLOW, true);
    y += 28;
    DrawTextPro(fonts, "RIGHT MOUSE", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Rotate View", col1X + 130, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "MOUSE WHEEL", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Zoom In/Out", col1X + 130, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "F", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Toggle Camera Follow", col1X + 130, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "1/2/3/4", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Camera Presets", col1X + 130, y, 14, LIGHTGRAY);
    y += 32;
    
    DrawTextPro(fonts, "ORBIT SELECTION", col1X, y, 16, YELLOW, true);
    y += 28;
    DrawTextPro(fonts, "TAB", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Cycle Active Orbit", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "Q-P Keys", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Toggle Orbits 1-10", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;

    DrawTextPro(fonts, "CTRL + V", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Show All Orbits", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;

    DrawTextPro(fonts, "CTRL + B", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Hide All Orbits", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;

    DrawTextPro(fonts, "CTRL + N", col1X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Show Active Orbits", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    
    
    y = panelY + 60;
    DrawTextPro(fonts, "DISPLAY OPTIONS", col2X, y, 16, YELLOW, true);
    y += 28;
    DrawTextPro(fonts, "H", col2X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Toggle Elements Panel", col2X + 50, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "C", col2X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Toggle Satellites List", col2X + 50, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "G", col2X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Toggle Grids", col2X + 50, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "M", col2X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Toggle Commands Menu", col2X + 50, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "X", col2X, y, 14, WHITE, true);
    DrawTextPro(fonts, "Show This Help", col2X + 50, y, 14, LIGHTGRAY);
    y += 32;
    
    DrawTextPro(fonts, "ORBIT FAMILIES", col2X, y, 16, YELLOW, true);
    y += 28;
    DrawTextPro(fonts, "LEO", col2X, y, 14, Color{100, 200, 255, 255}, true);
    DrawTextPro(fonts, "Low Earth (<2000 km)", col2X + 60, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "MEO", col2X, y, 14, Color{100, 255, 100, 255}, true);
    DrawTextPro(fonts, "Medium Earth (2-35k km)", col2X + 60, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "HEO", col2X, y, 14, Color{255, 150, 100, 255}, true);
    DrawTextPro(fonts, "High Elliptical", col2X + 60, y, 14, LIGHTGRAY);
    y += 22;
    DrawTextPro(fonts, "GEO", col2X, y, 14, Color{255, 100, 255, 255}, true);
    DrawTextPro(fonts, "Geostationary (~36k km)", col2X + 60, y, 14, LIGHTGRAY);
    
    y = panelY + panelH - 40;
    DrawTextPro(fonts, "Press X again to close", panelX + 230, y, 16, GRAY);
}

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    
    const int screenWidth = 1600;
    const int screenHeight = 900;
    
    InitWindow(screenWidth, screenHeight, "MDV - Mission Design Visualizer");
    SetTargetFPS(60);
    
    FontSystem fonts;
    fonts.load();
    
    EarthSystem earth;
    earth.load();
    
    Camera3D camera = { 0 };
    setCameraPreset(camera, PRESET_DEFAULT);
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
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
                  << " [" << satellites.back().stats.orbitFamily << "] "
                  << "(" << orbit.size() << " points)\n";
    }
    
    // Start all satellites hidden
    for (size_t i = 0; i < satellites.size(); i++) {
        satellites[i].visible = false;
    }
    
    int activeSatellite = 0;

    // Camera follow mode
    bool cameraFollowMode = false;
    Vector3 targetCameraPosition = camera.position;
    Vector3 targetCameraTarget = camera.target;
    float cameraTransitionSpeed = 5.0f;
    
    std::cout << "\n=== MDV Started ===\n";
    std::cout << "Press X for keyboard shortcuts\n";
    std::cout << "Press G to toggle grids\n";
    std::cout << "Press R to toggle Earth rotation\n";
    std::cout << "Press F to toggle camera follow mode\n";
    
    float animationSpeed = 1.0f;
    float animationAccumulator = 0.0f;

    bool showElements = true;
    bool showMessage = false;
    bool showList = false;
    bool showHelp = false;
    bool showGrids = true;
    bool earthRotation = true;  
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Update Earth rotation
        if (earthRotation) {
            earth.update(deltaTime);
        }

        // Update camera follow mode
        if (cameraFollowMode) {
            Vector3 satPos = toRaylib(satellites[activeSatellite].orbit[satellites[activeSatellite].currentFrame].position);

            // Update target positions
            float distance = 15.0f;
            Vector3 offset = { distance, distance * 0.7f, distance * 0.7f };

            targetCameraTarget = satPos;
            targetCameraPosition = Vector3{
                satPos.x + offset.x,
                satPos.y + offset.y,
                satPos.z + offset.z
            };

            // Smooth interpolation
            float lerpSpeed = cameraTransitionSpeed * deltaTime;

            camera.position.x += (targetCameraPosition.x - camera.position.x) * lerpSpeed;
            camera.position.y += (targetCameraPosition.y - camera.position.y) * lerpSpeed;
            camera.position.z += (targetCameraPosition.z - camera.position.z) * lerpSpeed;

            camera.target.x += (targetCameraTarget.x - camera.target.x) * lerpSpeed;
            camera.target.y += (targetCameraTarget.y - camera.target.y) * lerpSpeed;
            camera.target.z += (targetCameraTarget.z - camera.target.z) * lerpSpeed;
        }
        
        // Camera controls (ONLY when follow mode is OFF)
        if (!cameraFollowMode && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
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
        
        // Mouse wheel zoom (ONLY when follow mode is OFF)
        if (!cameraFollowMode) {
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
        }
        
        // Update animation
        if (animationSpeed > 0.01f) {
            animationAccumulator += animationSpeed;
            size_t framesToAdvance = static_cast<size_t>(animationAccumulator);
            animationAccumulator -= framesToAdvance;
            
            for (auto& sat : satellites) {
                if (sat.visible) {
                    sat.currentFrame = (sat.currentFrame + framesToAdvance) % sat.orbit.size();
                }
            }
        }
        
        // Controls
        if (IsKeyPressed(KEY_SPACE)) {
            animationSpeed = (animationSpeed > 0.01f) ? 0.0f : 1.0f;
        }
        if (IsKeyPressed(KEY_UP)) animationSpeed *= 2.0f;
        if (IsKeyPressed(KEY_DOWN)) animationSpeed *= 0.5f;
        if (animationSpeed > 10.0f) animationSpeed = 10.0f;
        if (animationSpeed < 0.05f && animationSpeed > 0.0f) animationSpeed = 0.05f;

        if (IsKeyPressed(KEY_M)) showMessage = !showMessage;
        if (IsKeyPressed(KEY_C)) showList = !showList;
        if (IsKeyPressed(KEY_X)) showHelp = !showHelp;
        if (IsKeyPressed(KEY_I)) showGrids = !showGrids;
        if (IsKeyPressed(KEY_R)) earthRotation = !earthRotation;
        
        if (IsKeyPressed(KEY_ONE)) setCameraPreset(camera, PRESET_DEFAULT);
        if (IsKeyPressed(KEY_TWO)) setCameraPreset(camera, PRESET_TOP);
        if (IsKeyPressed(KEY_THREE)) setCameraPreset(camera, PRESET_SIDE);
        if (IsKeyPressed(KEY_FOUR)) setCameraPreset(camera, PRESET_FRONT);
        
        if (IsKeyPressed(KEY_E)) showElements = !showElements;
        if (IsKeyPressed(KEY_F)) cameraFollowMode = !cameraFollowMode;
        
        // Toggle satellites
        if (IsKeyPressed(KEY_A) && satellites.size() > 0) satellites[0].visible = !satellites[0].visible;
        if (IsKeyPressed(KEY_S) && satellites.size() > 1) satellites[1].visible = !satellites[1].visible;
        if (IsKeyPressed(KEY_D) && satellites.size() > 2) satellites[2].visible = !satellites[2].visible;
        if (IsKeyPressed(KEY_F) && satellites.size() > 3) satellites[3].visible = !satellites[3].visible;
        if (IsKeyPressed(KEY_G) && satellites.size() > 4) satellites[4].visible = !satellites[4].visible;
        if (IsKeyPressed(KEY_H) && satellites.size() > 5) satellites[5].visible = !satellites[5].visible;
        if (IsKeyPressed(KEY_J) && satellites.size() > 6) satellites[6].visible = !satellites[6].visible;
        if (IsKeyPressed(KEY_K) && satellites.size() > 7) satellites[7].visible = !satellites[7].visible;
        if (IsKeyPressed(KEY_L) && satellites.size() > 8) satellites[8].visible = !satellites[8].visible;
        if (IsKeyPressed(KEY_Z) && satellites.size() > 9) satellites[9].visible = !satellites[9].visible;
        
        // Visibility bulk controls
        if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)){
            if(IsKeyPressed(KEY_V)) {
                // Show All Satellites
                for (auto& sat : satellites) {
                    sat.visible = true;
                }
            }
            if(IsKeyPressed(KEY_B)) {
                //Hide all satellites
                for (auto& sat : satellites) {
                    sat.visible = false;
                }
            }
            if (IsKeyPressed(KEY_N)) {
                // Solo Mode - show only active satellites
                for (size_t i = 0; i < satellites.size(); i++) {
                    satellites[i].visible = (i == activeSatellite);
                }
            }
        } 


        if (IsKeyPressed(KEY_TAB)) {
            int startingSat = activeSatellite;
            do {
                activeSatellite = (activeSatellite + 1) % satellites.size();
            } while (!satellites[activeSatellite].visible && activeSatellite != startingSat);

            // Camera follow on TAB press
            if (cameraFollowMode) {
                Vector3 satPos = toRaylib(satellites[activeSatellite].orbit[satellites[activeSatellite].currentFrame].position);

                // Calculate desired camera position (offset from satellite)
                float distance = 15.0f;
                Vector3 offset = { distance, distance * 0.7f, distance * 0.7f };

                targetCameraTarget = satPos;
                targetCameraPosition = Vector3{
                    satPos.x + offset.x,
                    satPos.y + offset.y,
                    satPos.z + offset.z
                };
            }
        }
        
        OrbitalElements currentElements = OrbitalElements::fromStateVector(
            satellites[activeSatellite].orbit[satellites[activeSatellite].currentFrame], 
            MU_EARTH
        );
        
        // Draw
        BeginDrawing();
        ClearBackground(Color{ 18, 3, 10 });
        
        BeginMode3D(camera);
        
            // Draw grids
            if (showGrids) {
                drawEquatorialGrid();
                drawReferenceCircles();
                drawOrbitalPlane(satellites[activeSatellite].orbit, 
                               satellites[activeSatellite].stats.familyColor);
            }
            
            // Draw Earth (textured and rotating)
            earth.draw();
            
            // Draw coordinate axes
            float axisLength = 10.0f;
            DrawLine3D(Vector3{0, 0, 0}, Vector3{axisLength, 0, 0}, RED);
            DrawLine3D(Vector3{0, 0, 0}, Vector3{0, axisLength, 0}, GREEN);
            DrawLine3D(Vector3{0, 0, 0}, Vector3{0, 0, axisLength}, SKYBLUE);
            
            // Draw satellites
            for (size_t s = 0; s < satellites.size(); s++) {
                if (!satellites[s].visible) continue;
                
                auto& sat = satellites[s];
                Color orbitColor = sat.preset.color;
                bool isActive = (s == activeSatellite);
                
                Color lineColor = isActive ? sat.stats.familyColor : Fade(sat.stats.familyColor, 0.4f);
                
                for (size_t i = 1; i < sat.orbit.size(); i++) {
                    Vector3 p1 = toRaylib(sat.orbit[i-1].position);
                    Vector3 p2 = toRaylib(sat.orbit[i].position);
                    DrawLine3D(p1, p2, lineColor);
                }
                
                Vector3 scPos = toRaylib(sat.orbit[sat.currentFrame].position);
                float satSize = isActive ? 0.4f : 0.25f;
                DrawSphere(scPos, satSize, orbitColor);
                
                if (isActive) {
                    Vector3D velScaled = sat.orbit[sat.currentFrame].velocity.normalized() * 2000.0;
                    Vector3 velEnd = toRaylib(sat.orbit[sat.currentFrame].position + velScaled);
                    DrawLine3D(scPos, velEnd, GREEN);
                }
                
                if (isActive) {
                    OrbitalElements elements = OrbitalElements::fromStateVector(sat.orbit[0], MU_EARTH);
                    if (elements.eccentricity > 0.01) {
                        size_t periIdx = 0;
                        DrawSphere(toRaylib(sat.orbit[periIdx].position), 0.3f, ORANGE);
                        
                        size_t apoIdx = sat.orbit.size() / 2;
                        DrawSphere(toRaylib(sat.orbit[apoIdx].position), 0.3f, PURPLE);
                        
                        DrawLine3D(
                            toRaylib(sat.orbit[periIdx].position),
                            toRaylib(sat.orbit[apoIdx].position),
                            Fade(WHITE, 0.3f)
                        );
                    }
                }
                
                if (sat.currentFrame > 25) {
                    for (size_t i = sat.currentFrame - 25; i < sat.currentFrame; i++) {
                        float alpha = (float)(i - (sat.currentFrame - 25)) / 25.0f;
                        Vector3 trailPos = toRaylib(sat.orbit[i].position);
                        DrawSphere(trailPos, 0.1f, Fade(orbitColor, alpha * 0.5f));
                    }
                }
            }
            
        EndMode3D();
        
        // UI
        DrawTextPro(fonts, "MISSION DESIGN VISUALIZER", 10, 10, 26, SKYBLUE, true);
        DrawTextPro(fonts, "Press X for help", screenWidth - 150, 10, 14, GRAY);
          
        int yPos = screenHeight - 28;
        const char* pauseStatus = (animationSpeed > 0.01f) ? "" : " [PAUSED]";
        const char* gridStatus = showGrids ? "Grids: ON" : "Grids: OFF";
        const char* rotStatus = earthRotation ? "Rotation: ON" : "Rotation: OFF";
        const char* followStatus = cameraFollowMode ? "Follow: ON" : "Follow: OFF";
        char statusText[300];
        snprintf(statusText, sizeof(statusText), "Speed: %.1fx%s | Active: %s [%s] | %s | %s | %s | FPS: %d", 
                 animationSpeed, pauseStatus,
                 satellites[activeSatellite].preset.name.c_str(),
                 satellites[activeSatellite].stats.orbitFamily.c_str(),
                 gridStatus, rotStatus, followStatus,
                 GetFPS());
        DrawTextPro(fonts, statusText, 10, yPos, 16, satellites[activeSatellite].stats.familyColor);
        
        // Satellite list
        if (showList) {
            int listX = 10;
            int listY = 50;
            int listW = 310;
            int listH = 45 + satellites.size() * 26;
            
            DrawRectangle(listX, listY, listW, listH, Fade(BLACK, 0.92f));
            DrawRectangleLines(listX, listY, listW, listH, SKYBLUE);
            
            listY += 12;
            listX += 12;
            
            DrawTextPro(fonts, "SATELLITES", listX, listY, 18, SKYBLUE, true);
            listY += 28;
            
            for (size_t i = 0; i < satellites.size(); i++) {
                Color textColor = satellites[i].visible ? satellites[i].stats.familyColor : GRAY;
                const char* activeMarker = (i == activeSatellite) ? "> " : "  ";
                char satText[128];
                snprintf(satText, sizeof(satText), "%s%s [%s]", 
                         activeMarker, 
                         satellites[i].preset.name.c_str(),
                         satellites[i].stats.orbitFamily.c_str());
                DrawTextPro(fonts, satText, listX, listY, 14, textColor);
                listY += 26;
            }
        }

        // Orbital elements panel
        if (showElements) {
            int panelX = screenWidth - 350;
            int panelY = 50;
            int panelW = 340;
            int panelH = 590;
            
            auto& activeSat = satellites[activeSatellite];
            
            DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.92f));
            DrawRectangleLines(panelX, panelY, panelW, panelH, activeSat.stats.familyColor);
            
            panelY += 12;
            panelX += 12;
            
            DrawTextPro(fonts, "ORBITAL ELEMENTS", panelX, panelY, 20, activeSat.stats.familyColor, true);
            panelY += 28;
            
            DrawTextPro(fonts, activeSat.preset.description.c_str(), 
                        panelX, panelY, 14, LIGHTGRAY);
            panelY += 26;
            
            DrawRectangle(panelX, panelY, 85, 24, activeSat.stats.familyColor);
            DrawTextPro(fonts, activeSat.stats.orbitFamily.c_str(), panelX + 10, panelY + 3, 16, BLACK, true);
            panelY += 32;
            
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Type: %s", currentElements.orbitType().c_str());
            DrawTextPro(fonts, buffer, panelX, panelY, 14, WHITE);
            panelY += 32;
            
            DrawTextPro(fonts, "ORBIT STATISTICS", panelX, panelY, 15, YELLOW, true);
            panelY += 22;
            
            DrawTextPro(fonts, "Altitude Range", panelX, panelY, 15, LIGHTGRAY);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  Periapsis: %.1f km", activeSat.stats.periapsisAlt);
            DrawTextPro(fonts, buffer, panelX, panelY, 15, ORANGE);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  Apoapsis:  %.1f km", activeSat.stats.apoapsisAlt);
            DrawTextPro(fonts, buffer, panelX, panelY, 15, PURPLE);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  Mean:      %.1f km", activeSat.stats.meanAltitude);
            DrawTextPro(fonts, buffer, panelX, panelY, 15, WHITE);
            panelY += 24;
            
            DrawTextPro(fonts, "Velocity Range", panelX, panelY, 15, LIGHTGRAY);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  At Periapsis: %.2f km/s", activeSat.stats.periapsisVel);
            DrawTextPro(fonts, buffer, panelX, panelY, 15, ORANGE);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  At Apoapsis:  %.2f km/s", activeSat.stats.apoapsisVel);
            DrawTextPro(fonts, buffer, panelX, panelY, 15, PURPLE);
            panelY += 18;
            
            double currentVel = activeSat.orbit[activeSat.currentFrame].velocity.magnitude();
            snprintf(buffer, sizeof(buffer), "  Current:      %.2f km/s", currentVel);
            DrawTextPro(fonts, buffer, panelX, panelY, 15, GREEN);
            panelY += 24;
            
            DrawTextPro(fonts, "CLASSICAL ELEMENTS", panelX, panelY, 15, YELLOW, true);
            panelY += 22;
            
            DrawTextPro(fonts, "Semi-major axis (a)", panelX, panelY, 15, LIGHTGRAY);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  %.2f km", currentElements.semiMajorAxis);
            DrawTextPro(fonts, buffer, panelX, panelY, 15, WHITE);
            panelY += 22;
            
            DrawTextPro(fonts, "Eccentricity (e)", panelX, panelY, 15, LIGHTGRAY);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  %.6f", currentElements.eccentricity);
            DrawTextPro(fonts, buffer, panelX, panelY, 15, WHITE);
            panelY += 22;
            
            DrawTextPro(fonts, "Inclination (i)", panelX, panelY, 15, LIGHTGRAY);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  %.2f deg", currentElements.inclinationDeg());
            DrawTextPro(fonts, buffer, panelX, panelY, 15, WHITE);
            panelY += 22;
            
            DrawTextPro(fonts, "RAAN (Omega)", panelX, panelY, 15, LIGHTGRAY);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  %.2f deg", currentElements.raanDeg());
            DrawTextPro(fonts, buffer, panelX, panelY, 15, WHITE);
            panelY += 22;
            
            DrawTextPro(fonts, "Arg. Periapsis (omega)", panelX, panelY, 15, LIGHTGRAY);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  %.2f deg", currentElements.argumentOfPeriapsisDeg());
            DrawTextPro(fonts, buffer, panelX, panelY, 15, WHITE);
            panelY += 22;
            
            DrawTextPro(fonts, "True Anomaly (nu)", panelX, panelY, 15, LIGHTGRAY);
            panelY += 18;
            snprintf(buffer, sizeof(buffer), "  %.2f deg", currentElements.trueAnomalyDeg());
            DrawTextPro(fonts, buffer, panelX, panelY, 15, WHITE);
            panelY += 22;
            
            snprintf(buffer, sizeof(buffer), "Period: %.2f min", currentElements.period / 60.0);
            DrawTextPro(fonts, buffer, panelX, panelY, 14, GREEN);
        }
        
        if (showHelp) {
            drawKeyboardLegend(fonts, screenWidth, screenHeight);
        }
        
        EndDrawing();
    }
    
    earth.unload();
    fonts.unload();
    CloseWindow();
    return 0;
}
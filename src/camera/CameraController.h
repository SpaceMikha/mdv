#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "raylib.h"
#include "Satellite.h"
#include <vector>

// Camera presets
enum CameraPreset {
    PRESET_DEFAULT,
    PRESET_TOP,
    PRESET_SIDE,
    PRESET_FRONT
};

// Camera controller
class CameraController {
public:
    CameraController();
    
    // Initialize camera
    void initialize();
    
    // Update camera (handles follow mode, transitions, etc.)
    void update(float deltaTime, const std::vector<Satellite>& satellites, size_t activeSatIndex);
    
    // Handle manual controls (mouse)
    void handleManualControls();
    
    // Get raylib camera
    Camera3D& getCamera() { return camera; }
    const Camera3D& getCamera() const { return camera; }
    
    // Set camera preset
    void setPreset(CameraPreset preset);
    
    // Follow mode
    void setFollowMode(bool enabled);
    bool isFollowModeEnabled() const { return followMode; }
    void toggleFollowMode() { followMode = !followMode; }
    
    // Transitioning state
    bool isTransitioning() const { return transitioning; }
    
    // Mouse over UI control
    void setMouseOverUI(bool overUI) { mouseOverUI = overUI; }
    
private:
    Camera3D camera;
    bool followMode;
    bool transitioning;
    bool mouseOverUI;
    Vector3 targetPosition;
    Vector3 targetTarget;
    float transitionSpeed;
    
    void updateFollowMode(float deltaTime, const Satellite& activeSat);
    void updateTransition(float deltaTime);
    void updateManualRotation();
    void updateZoom();
};

#endif // CAMERA_CONTROLLER_H
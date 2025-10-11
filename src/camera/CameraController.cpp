#include "CameraController.h"
#include "RenderUtils.h"
#include "Constants.h"
#include <cmath>

CameraController::CameraController()
    : followMode(false), transitioning(false), transitionSpeed(CAMERA_TRANSITION_SPEED),
      mouseOverUI(false) {}

void CameraController::initialize() {
    camera.position = Vector3{ 20.0f, 20.0f, 20.0f };
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    targetPosition = camera.position;
    targetTarget = camera.target;
}

void CameraController::update(float deltaTime, const std::vector<Satellite>& satellites, size_t activeSatIndex) {
    if (followMode && activeSatIndex < satellites.size()) {
        updateFollowMode(deltaTime, satellites[activeSatIndex]);
    } else if (transitioning) {
        updateTransition(deltaTime);
    }
}

void CameraController::handleManualControls() {
    if (!followMode && !transitioning) {
        updateManualRotation();
        updateZoom();
    }
}

void CameraController::updateFollowMode(float deltaTime, const Satellite& activeSat) {
    Vector3 satPos = RenderUtils::toRaylib(activeSat.getCurrentState().position);
    
    // Dynamic distance based on orbit altitude
    double meanAlt = activeSat.getStats().meanAltitude;
    float distance;
    if (meanAlt < 2000.0) {
        distance = 12.0f;
    } else if (meanAlt < 10000.0) {
        distance = 18.0f;
    } else {
        distance = 25.0f;
    }
    
    Vector3 offset = { distance, distance * 0.7f, distance * 0.7f };
    
    targetTarget = satPos;
    targetPosition = Vector3{
        satPos.x + offset.x,
        satPos.y + offset.y,
        satPos.z + offset.z
    };
    
    // Apply smooth following
    float lerpSpeed = transitionSpeed * deltaTime;
    camera.position.x += (targetPosition.x - camera.position.x) * lerpSpeed;
    camera.position.y += (targetPosition.y - camera.position.y) * lerpSpeed;
    camera.position.z += (targetPosition.z - camera.position.z) * lerpSpeed;
    
    camera.target.x += (targetTarget.x - camera.target.x) * lerpSpeed;
    camera.target.y += (targetTarget.y - camera.target.y) * lerpSpeed;
    camera.target.z += (targetTarget.z - camera.target.z) * lerpSpeed;
}

void CameraController::updateTransition(float deltaTime) {
    float lerpSpeed = transitionSpeed * deltaTime;
    
    camera.position.x += (targetPosition.x - camera.position.x) * lerpSpeed;
    camera.position.y += (targetPosition.y - camera.position.y) * lerpSpeed;
    camera.position.z += (targetPosition.z - camera.position.z) * lerpSpeed;
    
    camera.target.x += (targetTarget.x - camera.target.x) * lerpSpeed;
    camera.target.y += (targetTarget.y - camera.target.y) * lerpSpeed;
    camera.target.z += (targetTarget.z - camera.target.z) * lerpSpeed;
    
    // Check if transition is complete
    float posDistance = sqrtf(
        (targetPosition.x - camera.position.x) * (targetPosition.x - camera.position.x) +
        (targetPosition.y - camera.position.y) * (targetPosition.y - camera.position.y) +
        (targetPosition.z - camera.position.z) * (targetPosition.z - camera.position.z)
    );
    
    if (posDistance < 0.1f) {
        transitioning = false;
    }
}

void CameraController::updateManualRotation() {
    if (mouseOverUI) return;
    if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) return;
    
    Vector2 mouseDelta = GetMouseDelta();
    float rotationSpeed = 0.003f;
    
    Vector3 direction = {
        camera.position.x - camera.target.x,
        camera.position.y - camera.target.y,
        camera.position.z - camera.target.z
    };
    float radius = sqrtf(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
    
    float angle = atan2f(direction.z, direction.x);
    angle -= mouseDelta.x * rotationSpeed;
    
    float elevation = asinf(direction.y / radius);
    elevation -= mouseDelta.y * rotationSpeed;
    
    if (elevation > 1.5f) elevation = 1.5f;
    if (elevation < -1.5f) elevation = -1.5f;
    
    camera.position.x = camera.target.x + radius * cosf(elevation) * cosf(angle);
    camera.position.y = camera.target.y + radius * sinf(elevation);
    camera.position.z = camera.target.z + radius * cosf(elevation) * sinf(angle);
    
    // Update targets when manually moving
    targetPosition = camera.position;
    targetTarget = camera.target;
}

void CameraController::updateZoom() {
    if (mouseOverUI) return;
    
    float wheel = GetMouseWheelMove();
    if (wheel == 0) return;
    
    Vector3 direction = {
        camera.position.x - camera.target.x,
        camera.position.y - camera.target.y,
        camera.position.z - camera.target.z
    };
    float distance = sqrtf(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
    
    float zoomSpeed = distance * 0.1f;
    distance -= wheel * zoomSpeed;
    
    if (distance < CAMERA_MIN_DISTANCE) distance = CAMERA_MIN_DISTANCE;
    if (distance > CAMERA_MAX_DISTANCE) distance = CAMERA_MAX_DISTANCE;
    
    float currentDist = sqrtf(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
    direction.x = direction.x / currentDist * distance;
    direction.y = direction.y / currentDist * distance;
    direction.z = direction.z / currentDist * distance;
    
    camera.position.x = camera.target.x + direction.x;
    camera.position.y = camera.target.y + direction.y;
    camera.position.z = camera.target.z + direction.z;
    
    targetPosition = camera.position;
}

void CameraController::setPreset(CameraPreset preset) {
    targetTarget = Vector3{ 0.0f, 0.0f, 0.0f };
    
    switch (preset) {
        case PRESET_TOP:
            targetPosition = Vector3{ 0.0f, 35.0f, 0.01f };
            camera.up = Vector3{ 0.0f, 0.0f, -1.0f };
            break;
        case PRESET_SIDE:
            targetPosition = Vector3{ 35.0f, 0.0f, 0.0f };
            camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
            break;
        case PRESET_FRONT:
            targetPosition = Vector3{ 0.0f, 0.0f, 35.0f };
            camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
            break;
        case PRESET_DEFAULT:
        default:
            targetPosition = Vector3{ 20.0f, 20.0f, 20.0f };
            camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
            break;
    }
    
    transitioning = true;
}

void CameraController::setFollowMode(bool enabled) {
    followMode = enabled;
    
    // Reset camera to default when disabling follow mode
    if (!followMode) {
        targetPosition = Vector3{ 20.0f, 20.0f, 20.0f };
        targetTarget = Vector3{ 0.0f, 0.0f, 0.0f };
        transitioning = true;
    }
}
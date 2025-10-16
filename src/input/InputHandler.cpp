#include "InputHandler.h"
#include "Constants.h"
#include "raylib.h"

void InputHandler::processInput(
    std::vector<Satellite>& satellites,
    size_t& activeSatelliteIndex,
    CameraController& camera,
    UIManager& ui,
    float& animationSpeed,
    bool& showGrids,
    bool& earthRotation,
    ForceModel& forceModel  // NEW PARAMETER
) {
    handleAnimationControls(animationSpeed);
    handleCameraControls(camera);
    handleSatelliteToggle(satellites);
    handleSatelliteBulkControls(satellites, activeSatelliteIndex);
    handleSatelliteCycle(satellites, activeSatelliteIndex, camera);
    handleUIToggles(ui, showGrids, earthRotation);
    handleForceModelToggles(forceModel);  // NEW CALL
}

void InputHandler::handleAnimationControls(float& animationSpeed) {
    if (IsKeyPressed(KEY_SPACE)) {
        animationSpeed = (animationSpeed > 0.01f) ? 0.0f : 1.0f;
    }
    if (IsKeyPressed(KEY_UP)) animationSpeed *= 2.0f;
    if (IsKeyPressed(KEY_DOWN)) animationSpeed *= 0.5f;
    
    if (animationSpeed > MAX_ANIMATION_SPEED) animationSpeed = MAX_ANIMATION_SPEED;
    if (animationSpeed < MIN_ANIMATION_SPEED && animationSpeed > 0.0f) 
        animationSpeed = MIN_ANIMATION_SPEED;
}

void InputHandler::handleCameraControls(CameraController& camera) {
    if (IsKeyPressed(KEY_F)) {
        camera.toggleFollowMode();
    }
    
    if (IsKeyPressed(KEY_ONE)) {
        camera.setPreset(PRESET_DEFAULT);
    }
    if (IsKeyPressed(KEY_TWO)) {
        camera.setPreset(PRESET_TOP);
    }
    if (IsKeyPressed(KEY_THREE)) {
        camera.setPreset(PRESET_SIDE);
    }
    if (IsKeyPressed(KEY_FOUR)) {
        camera.setPreset(PRESET_FRONT);
    }
}

void InputHandler::handleSatelliteToggle(std::vector<Satellite>& satellites) {
    if (IsKeyPressed(KEY_Q) && satellites.size() > 0) 
        satellites[0].setVisible(!satellites[0].isVisible());
    if (IsKeyPressed(KEY_W) && satellites.size() > 1) 
        satellites[1].setVisible(!satellites[1].isVisible());
    if (IsKeyPressed(KEY_A) && satellites.size() > 2) 
        satellites[2].setVisible(!satellites[2].isVisible());
    if (IsKeyPressed(KEY_S) && satellites.size() > 3) 
        satellites[3].setVisible(!satellites[3].isVisible());
    if (IsKeyPressed(KEY_D) && satellites.size() > 4) 
        satellites[4].setVisible(!satellites[4].isVisible());
    if (IsKeyPressed(KEY_H) && satellites.size() > 5) 
        satellites[5].setVisible(!satellites[5].isVisible());
    if (IsKeyPressed(KEY_J) && satellites.size() > 6) 
        satellites[6].setVisible(!satellites[6].isVisible());
    if (IsKeyPressed(KEY_K) && satellites.size() > 7) 
        satellites[7].setVisible(!satellites[7].isVisible());
    if (IsKeyPressed(KEY_L) && satellites.size() > 8) 
        satellites[8].setVisible(!satellites[8].isVisible());
    if (IsKeyPressed(KEY_Z) && satellites.size() > 9) 
        satellites[9].setVisible(!satellites[9].isVisible());
}

void InputHandler::handleSatelliteBulkControls(
    std::vector<Satellite>& satellites, 
    size_t activeSatelliteIndex
) {
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
        if (IsKeyPressed(KEY_V)) {
            for (auto& sat : satellites) {
                sat.setVisible(true);
            }
        }
        if (IsKeyPressed(KEY_B)) {
            for (auto& sat : satellites) {
                sat.setVisible(false);
            }
        }
        if (IsKeyPressed(KEY_N)) {
            for (size_t i = 0; i < satellites.size(); i++) {
                satellites[i].setVisible(i == activeSatelliteIndex);
            }
        }
    }
}

void InputHandler::handleSatelliteCycle(
    std::vector<Satellite>& satellites,
    size_t& activeSatelliteIndex,
    CameraController& camera
) {
    if (!IsKeyPressed(KEY_TAB)) return;
    if (satellites.empty()) return;
    
    size_t startingSat = activeSatelliteIndex;
    do {
        activeSatelliteIndex = (activeSatelliteIndex + 1) % satellites.size();
    } while (!satellites[activeSatelliteIndex].isVisible() && 
             activeSatelliteIndex != startingSat);
}

void InputHandler::handleUIToggles(UIManager& ui, bool& showGrids, bool& earthRotation) {
    if (IsKeyPressed(KEY_E)) ui.toggleElements();
    if (IsKeyPressed(KEY_C)) ui.toggleList();
    if (IsKeyPressed(KEY_X)) ui.toggleHelp();
    if (IsKeyPressed(KEY_G)) showGrids = !showGrids;
    if (IsKeyPressed(KEY_R)) earthRotation = !earthRotation;
    if (IsKeyPressed(KEY_V)) ui.toggleEclipse();
    if (IsKeyPressed(KEY_Y)) ui.toggleSolar();
}

// NEW FUNCTION - Handle force model toggles
void InputHandler::handleForceModelToggles(ForceModel& forceModel) {
    if (IsKeyPressed(KEY_M)) {
        forceModel.j2Perturbation = !forceModel.j2Perturbation;
    }
    
    // Future: Add more force model toggles here
    // if (IsKeyPressed(KEY_N)) { forceModel.atmosphericDrag = !forceModel.atmosphericDrag; }
}
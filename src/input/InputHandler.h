#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Satellite.h"
#include "CameraController.h"
#include "UIManager.h"
#include "ForceModel.h"
#include <vector>

// Input handler - processes all keyboard and mouse input
class InputHandler {
public:
    // Process all input for this frame
    void processInput(
        std::vector<Satellite>& satellites,
        size_t& activeSatelliteIndex,
        CameraController& camera,
        UIManager& ui,
        float& animationSpeed,
        bool& showGrids,
        bool& earthRotation,
        ForceModel& forceModel
    );
    
private:
    void handleAnimationControls(float& animationSpeed);
    void handleCameraControls(CameraController& camera);
    void handleSatelliteToggle(std::vector<Satellite>& satellites);
    void handleSatelliteBulkControls(std::vector<Satellite>& satellites, size_t activeSatelliteIndex);
    void handleSatelliteCycle(std::vector<Satellite>& satellites, size_t& activeSatelliteIndex, CameraController& camera);
    void handleUIToggles(UIManager& ui, bool& showGrids, bool& earthRotation);
    void handleForceModelToggles(ForceModel& forceModel);
};

#endif // INPUT_HANDLER_H
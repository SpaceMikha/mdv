#include "UIManager.h"
#include "Eclipse.h"
#include "Constants.h"
#include <cstdio>

UIManager::UIManager(int width, int height)
    : screenWidth(width), screenHeight(height),
      showElements(true), showList(false), showHelp(false), showEclipse(true) {}

void UIManager::draw(
    const FontSystem& fonts,
    const std::vector<Satellite>& satellites,
    size_t activeSatIndex,
    const OrbitalElements& currentElements,
    float animationSpeed,
    bool showGrids,
    bool earthRotation,
    bool cameraFollow,
    int fps,
    const Vector3D& sunDirection
) {
    drawTitle(fonts);
    
    if (activeSatIndex < satellites.size()) {
        drawStatusBar(fonts, satellites[activeSatIndex], animationSpeed, 
                     showGrids, earthRotation, cameraFollow, fps);
        
        if (showList) {
            drawSatelliteList(fonts, satellites, activeSatIndex);
        }
        
        if (showElements) {
            drawOrbitalElements(fonts, satellites[activeSatIndex], currentElements, sunDirection);
        }
    }
    
    if (showHelp) {
        drawKeyboardLegend(fonts);
    }
}

void UIManager::drawTitle(const FontSystem& fonts) {
    fonts.drawText("MISSION DESIGN VISUALIZER", 10, 10, 26, SKYBLUE, true);
    fonts.drawText("Press X for help", screenWidth - 150, 10, 14, GRAY);
}

void UIManager::drawStatusBar(
    const FontSystem& fonts,
    const Satellite& activeSat,
    float animationSpeed,
    bool showGrids,
    bool earthRotation,
    bool cameraFollow,
    int fps
) {
    int yPos = screenHeight - 28;
    const char* pauseStatus = (animationSpeed > 0.01f) ? "" : " [PAUSED]";
    const char* gridStatus = showGrids ? "Grids: ON" : "Grids: OFF";
    const char* rotStatus = earthRotation ? "Rotation: ON" : "Rotation: OFF";
    const char* followStatus = cameraFollow ? "Follow: ON" : "Follow: OFF";
    
    char statusText[300];
    snprintf(statusText, sizeof(statusText), 
             "Speed: %.1fx%s | Active: %s [%s] | %s | %s | %s | FPS: %d", 
             animationSpeed, pauseStatus,
             activeSat.getPreset().name.c_str(),
             activeSat.getStats().orbitFamily.c_str(),
             gridStatus, rotStatus, followStatus, fps);
    
    fonts.drawText(statusText, 10, yPos, 16, activeSat.getStats().familyColor);
}

void UIManager::drawSatelliteList(
    const FontSystem& fonts,
    const std::vector<Satellite>& satellites,
    size_t activeSatIndex
) {
    int listX = 10;
    int listY = 50;
    int listW = 310;
    int listH = 45 + satellites.size() * 26;
    
    DrawRectangle(listX, listY, listW, listH, Fade(BLACK, 0.92f));
    DrawRectangleLines(listX, listY, listW, listH, SKYBLUE);
    
    listY += 12;
    listX += 12;
    
    fonts.drawText("SATELLITES", listX, listY, 18, SKYBLUE, true);
    listY += 28;
    
    for (size_t i = 0; i < satellites.size(); i++) {
        Color textColor = satellites[i].isVisible() ? 
            satellites[i].getStats().familyColor : GRAY;
        const char* activeMarker = (i == activeSatIndex) ? "> " : "  ";
        
        char satText[128];
        snprintf(satText, sizeof(satText), "%s%s [%s]", 
                 activeMarker, 
                 satellites[i].getPreset().name.c_str(),
                 satellites[i].getStats().orbitFamily.c_str());
        
        fonts.drawText(satText, listX, listY, 14, textColor);
        listY += 26;
    }
}

void UIManager::drawOrbitalElements(
    const FontSystem& fonts,
    const Satellite& activeSat,
    const OrbitalElements& elements,
    const Vector3D& sunDirection
) {
    int panelX = screenWidth - 350;
    int panelY = 50;
    int panelW = 340;
    int panelH = 650;
    
    DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.92f));
    DrawRectangleLines(panelX, panelY, panelW, panelH, activeSat.getStats().familyColor);
    
    panelY += 12;
    panelX += 12;
    
    fonts.drawText("ORBITAL ELEMENTS", panelX, panelY, 20, activeSat.getStats().familyColor, true);
    panelY += 28;
    
    fonts.drawText(activeSat.getPreset().description.c_str(), panelX, panelY, 14, LIGHTGRAY);
    panelY += 26;
    
    DrawRectangle(panelX, panelY, 85, 24, activeSat.getStats().familyColor);
    fonts.drawText(activeSat.getStats().orbitFamily.c_str(), panelX + 10, panelY + 3, 16, BLACK, true);
    panelY += 32;
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Type: %s", elements.orbitType().c_str());
    fonts.drawText(buffer, panelX, panelY, 14, WHITE);
    panelY += 22;
    
    // Eclipse status
    if (showEclipse) {
        EclipseStatus eclipse = EclipseDetector::checkEclipse(
            activeSat.getCurrentState().position,
            sunDirection,
            EARTH_RADIUS
        );
        
        const char* eclipseText = "Sunlit";
        Color eclipseColor = YELLOW;
        
        if (eclipse.inUmbra) {
            eclipseText = "UMBRA (Full Shadow)";
            eclipseColor = RED;
        } else if (eclipse.inPenumbra) {
            eclipseText = "PENUMBRA (Partial)";
            eclipseColor = ORANGE;
        }
        
        fonts.drawText("Eclipse Status:", panelX, panelY, 14, LIGHTGRAY);
        panelY += 18;
        snprintf(buffer, sizeof(buffer), "  %s", eclipseText);
        fonts.drawText(buffer, panelX, panelY, 14, eclipseColor);
        panelY += 22;
    }
    
    panelY += 10;
    
    fonts.drawText("ORBIT STATISTICS", panelX, panelY, 15, YELLOW, true);
    panelY += 22;
    
    fonts.drawText("Altitude Range", panelX, panelY, 15, LIGHTGRAY);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  Periapsis: %.1f km", activeSat.getStats().periapsisAlt);
    fonts.drawText(buffer, panelX, panelY, 15, ORANGE);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  Apoapsis:  %.1f km", activeSat.getStats().apoapsisAlt);
    fonts.drawText(buffer, panelX, panelY, 15, PURPLE);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  Mean:      %.1f km", activeSat.getStats().meanAltitude);
    fonts.drawText(buffer, panelX, panelY, 15, WHITE);
    panelY += 24;
    
    fonts.drawText("Velocity Range", panelX, panelY, 15, LIGHTGRAY);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  At Periapsis: %.2f km/s", activeSat.getStats().periapsisVel);
    fonts.drawText(buffer, panelX, panelY, 15, ORANGE);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  At Apoapsis:  %.2f km/s", activeSat.getStats().apoapsisVel);
    fonts.drawText(buffer, panelX, panelY, 15, PURPLE);
    panelY += 18;
    
    double currentVel = activeSat.getCurrentState().velocity.magnitude();
    snprintf(buffer, sizeof(buffer), "  Current:      %.2f km/s", currentVel);
    fonts.drawText(buffer, panelX, panelY, 15, GREEN);
    panelY += 24;
    
    fonts.drawText("CLASSICAL ELEMENTS", panelX, panelY, 15, YELLOW, true);
    panelY += 22;
    
    fonts.drawText("Semi-major axis (a)", panelX, panelY, 15, LIGHTGRAY);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  %.2f km", elements.semiMajorAxis);
    fonts.drawText(buffer, panelX, panelY, 15, WHITE);
    panelY += 22;
    
    fonts.drawText("Eccentricity (e)", panelX, panelY, 15, LIGHTGRAY);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  %.6f", elements.eccentricity);
    fonts.drawText(buffer, panelX, panelY, 15, WHITE);
    panelY += 22;
    
    fonts.drawText("Inclination (i)", panelX, panelY, 15, LIGHTGRAY);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  %.2f deg", elements.inclinationDeg());
    fonts.drawText(buffer, panelX, panelY, 15, WHITE);
    panelY += 22;
    
    fonts.drawText("RAAN (Omega)", panelX, panelY, 15, LIGHTGRAY);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  %.2f deg", elements.raanDeg());
    fonts.drawText(buffer, panelX, panelY, 15, WHITE);
    panelY += 22;
    
    fonts.drawText("Arg. Periapsis (omega)", panelX, panelY, 15, LIGHTGRAY);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  %.2f deg", elements.argumentOfPeriapsisDeg());
    fonts.drawText(buffer, panelX, panelY, 15, WHITE);
    panelY += 22;
    
    fonts.drawText("True Anomaly (nu)", panelX, panelY, 15, LIGHTGRAY);
    panelY += 18;
    snprintf(buffer, sizeof(buffer), "  %.2f deg", elements.trueAnomalyDeg());
    fonts.drawText(buffer, panelX, panelY, 15, WHITE);
    panelY += 22;
    
    snprintf(buffer, sizeof(buffer), "Period: %.2f min", elements.period / 60.0);
    fonts.drawText(buffer, panelX, panelY, 14, GREEN);
}

void UIManager::drawKeyboardLegend(const FontSystem& fonts) {
    int panelW = 650;
    int panelH = 680;
    int panelX = (screenWidth - panelW) / 2;
    int panelY = (screenHeight - panelH) / 2;
    
    DrawRectangle(panelX, panelY, panelW, panelH, Fade(BLACK, 0.95f));
    DrawRectangleLines(panelX, panelY, panelW, panelH, SKYBLUE);
    
    int x = panelX + 20;
    int y = panelY + 15;
    
    fonts.drawText("KEYBOARD SHORTCUTS", x + 160, y, 24, SKYBLUE, true);
    y += 45;
    
    int col1X = x;
    int col2X = x + 320;
    
    fonts.drawText("SIMULATION CONTROL", col1X, y, 16, YELLOW, true);
    y += 28;
    fonts.drawText("SPACE", col1X, y, 14, WHITE, true);
    fonts.drawText("Pause/Resume", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("UP/DOWN", col1X, y, 14, WHITE, true);
    fonts.drawText("Speed Up/Down", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("R", col1X, y, 14, WHITE, true);
    fonts.drawText("Toggle Earth Rotation", col1X + 90, y, 14, LIGHTGRAY);
    y += 32;
    
    fonts.drawText("CAMERA CONTROL", col1X, y, 16, YELLOW, true);
    y += 28;
    fonts.drawText("RIGHT MOUSE", col1X, y, 14, WHITE, true);
    fonts.drawText("Rotate View", col1X + 130, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("MOUSE WHEEL", col1X, y, 14, WHITE, true);
    fonts.drawText("Zoom In/Out", col1X + 130, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("F", col1X, y, 14, WHITE, true);
    fonts.drawText("Toggle Camera Follow", col1X + 130, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("1/2/3/4", col1X, y, 14, WHITE, true);
    fonts.drawText("Camera Presets", col1X + 130, y, 14, LIGHTGRAY);
    y += 32;
    
    fonts.drawText("ORBIT SELECTION", col1X, y, 16, YELLOW, true);
    y += 28;
    fonts.drawText("TAB", col1X, y, 14, WHITE, true);
    fonts.drawText("Cycle Active Orbit", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("Q-P Keys", col1X, y, 14, WHITE, true);
    fonts.drawText("Toggle Orbits 1-10", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("CTRL+V", col1X, y, 14, WHITE, true);
    fonts.drawText("Show All Orbits", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("CTRL+B", col1X, y, 14, WHITE, true);
    fonts.drawText("Hide All Orbits", col1X + 90, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("CTRL+N", col1X, y, 14, WHITE, true);
    fonts.drawText("Solo Active Orbit", col1X + 90, y, 14, LIGHTGRAY);
    y += 32;
    
    y = panelY + 60;
    fonts.drawText("DISPLAY OPTIONS", col2X, y, 16, YELLOW, true);
    y += 28;
    fonts.drawText("E", col2X, y, 14, WHITE, true);
    fonts.drawText("Toggle Elements Panel", col2X + 50, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("C", col2X, y, 14, WHITE, true);
    fonts.drawText("Toggle Satellites List", col2X + 50, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("G", col2X, y, 14, WHITE, true);
    fonts.drawText("Toggle Grids", col2X + 50, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("M", col2X, y, 14, WHITE, true);
    fonts.drawText("Toggle Commands Menu", col2X + 50, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("X", col2X, y, 14, WHITE, true);
    fonts.drawText("Show This Help", col2X + 50, y, 14, LIGHTGRAY);
    y += 32;
    
    fonts.drawText("ORBIT FAMILIES", col2X, y, 16, YELLOW, true);
    y += 28;
    fonts.drawText("LEO", col2X, y, 14, Color{100, 200, 255, 255}, true);
    fonts.drawText("Low Earth (<2000 km)", col2X + 60, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("MEO", col2X, y, 14, Color{100, 255, 100, 255}, true);
    fonts.drawText("Medium Earth (2-35k km)", col2X + 60, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("HEO", col2X, y, 14, Color{255, 150, 100, 255}, true);
    fonts.drawText("High Elliptical", col2X + 60, y, 14, LIGHTGRAY);
    y += 22;
    fonts.drawText("GEO", col2X, y, 14, Color{255, 100, 255, 255}, true);
    fonts.drawText("Geostationary (~36k km)", col2X + 60, y, 14, LIGHTGRAY);
    
    y = panelY + panelH - 40;
    fonts.drawText("Press X again to close", panelX + 230, y, 16, GRAY);
}
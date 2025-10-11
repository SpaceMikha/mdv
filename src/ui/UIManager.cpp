#include "UIManager.h"
#include "Eclipse.h"
#include "GroundTrack.h"  
#include "Constants.h"
#include "GroundStation.h"
#include <cstdio>
#include <cmath>

UIManager::UIManager(int width, int height)
    : screenWidth(width), screenHeight(height),
      showLeftSidebar(true), showRightSidebar(true), showHelp(false),
      showEclipse(true), showSolar(true), showGroundStations(true),
      leftSidebarOffset(0.0f), rightSidebarOffset(0.0f),
      targetLeftOffset(0.0f), targetRightOffset(0.0f),
      leftSidebarScroll(0.0f), leftSidebarContentHeight(0.0f) {}

void UIManager::update(float deltaTime) {
    // Animate left sidebar
    float leftTarget = showLeftSidebar ? 0.0f : -(float)UITheme::SIDEBAR_WIDTH;
    if (fabs(leftSidebarOffset - leftTarget) > 0.5f) {
        float speed = 1200.0f * deltaTime; // pixels per second
        if (leftSidebarOffset < leftTarget) {
            leftSidebarOffset += speed;
            if (leftSidebarOffset > leftTarget) leftSidebarOffset = leftTarget;
        } else {
            leftSidebarOffset -= speed;
            if (leftSidebarOffset < leftTarget) leftSidebarOffset = leftTarget;
        }
    } else {
        leftSidebarOffset = leftTarget;
    }
    
    // Animate right sidebar
    float rightTarget = showRightSidebar ? 0.0f : (float)UITheme::SIDEBAR_WIDTH;
    if (fabs(rightSidebarOffset - rightTarget) > 0.5f) {
        float speed = 1200.0f * deltaTime;
        if (rightSidebarOffset < rightTarget) {
            rightSidebarOffset += speed;
            if (rightSidebarOffset > rightTarget) rightSidebarOffset = rightTarget;
        } else {
            rightSidebarOffset -= speed;
            if (rightSidebarOffset < rightTarget) rightSidebarOffset = rightTarget;
        }
    } else {
        rightSidebarOffset = rightTarget;
    }
}

void UIManager::draw(
    const FontSystem &fonts,
    const std::vector<Satellite> &satellites,
    size_t activeSatIndex,
    const OrbitalElements &currentElements,
    float animationSpeed,
    bool showGrids,
    bool earthRotation,
    bool cameraFollow,
    int fps,
    const Vector3D &sunDirection,
    const std::vector<GroundStation> &groundStations,
    const std::vector<AccessStatistics> &accessStats)
{
    drawTitleBar(fonts);
    
    if (activeSatIndex < satellites.size()) {
        drawStatusBar(fonts, satellites[activeSatIndex], animationSpeed,
                      showGrids, earthRotation, cameraFollow, fps);
        
        // Draw sidebars with animation offset
        if (leftSidebarOffset > -(float)UITheme::SIDEBAR_WIDTH + 10.0f) {
            drawLeftSidebar(fonts, satellites, activeSatIndex, sunDirection,
                           groundStations, accessStats);
        }
        
        if (rightSidebarOffset < (float)UITheme::SIDEBAR_WIDTH - 10.0f) {
            drawRightSidebar(fonts, satellites[activeSatIndex], currentElements, sunDirection);
        }
    }
    
    if (showHelp) {
        drawKeyboardLegend(fonts);
    }
}

void UIManager::drawTitleBar(const FontSystem &fonts) {
    // Background
    DrawRectangle(0, 0, screenWidth, UITheme::TITLE_BAR_HEIGHT, UITheme::BG_PANEL);
    DrawLineEx(
        Vector2{0, (float)UITheme::TITLE_BAR_HEIGHT},
        Vector2{(float)screenWidth, (float)UITheme::TITLE_BAR_HEIGHT},
        2.0f,
        UITheme::BORDER_ACCENT
    );
    
    // Title
    fonts.drawText("MISSION DESIGN VISUALIZER", 
                   UITheme::SPACING_LG, 
                   (UITheme::TITLE_BAR_HEIGHT - 28) / 2, 
                   UITheme::FONT_SIZE_TITLE, 
                   UITheme::SECONDARY, 
                   true);
    
    // Version and status indicators
    int rightX = screenWidth - UITheme::SPACING_LG;
    
    fonts.drawText("v0.8.3", 
                   rightX - 60, 
                   (UITheme::TITLE_BAR_HEIGHT - 12) / 2, 
                   UITheme::FONT_SIZE_SMALL, 
                   UITheme::TEXT_MUTED);
    
    // Status indicators (Eclipse, Solar)
    rightX -= 100;
    if (showEclipse) {
        fonts.drawText("[Eclipse]", rightX - 80, 
                       (UITheme::TITLE_BAR_HEIGHT - 14) / 2,
                       UITheme::FONT_SIZE_BODY, UITheme::WARNING);
        rightX -= 90;
    }
    if (showSolar) {
        fonts.drawText("[Solar]", rightX - 60, 
                       (UITheme::TITLE_BAR_HEIGHT - 14) / 2,
                       UITheme::FONT_SIZE_BODY, UITheme::ACCENT);
    }
}

void UIManager::drawStatusBar(
    const FontSystem &fonts,
    const Satellite &activeSat,
    float animationSpeed,
    bool showGrids,
    bool earthRotation,
    bool cameraFollow,
    int fps)
{
    int yPos = screenHeight - UITheme::STATUS_BAR_HEIGHT;
    
    // Background
    DrawRectangle(0, yPos, screenWidth, UITheme::STATUS_BAR_HEIGHT, UITheme::BG_PANEL);
    DrawLineEx(
        Vector2{0, (float)yPos},
        Vector2{(float)screenWidth, (float)yPos},
        2.0f,
        UITheme::BORDER
    );
    
    int xPos = UITheme::SPACING_LG;
    int yText = yPos + (UITheme::STATUS_BAR_HEIGHT - 16) / 2;
    
    // Speed indicator
    char buffer[128];
    const char* pauseStatus = (animationSpeed > 0.01f) ? "" : " PAUSED";
    snprintf(buffer, sizeof(buffer), "Speed: %.1fx%s", animationSpeed, pauseStatus);
    Color speedColor = (animationSpeed > 0.01f) ? UITheme::TEXT_PRIMARY : UITheme::WARNING;
    fonts.drawText(buffer, xPos, yText, UITheme::FONT_SIZE_BODY, speedColor, true);
    xPos += 150;
    
    // Separator
    DrawLineEx(
        Vector2{(float)xPos, (float)(yPos + 8)},
        Vector2{(float)xPos, (float)(yPos + UITheme::STATUS_BAR_HEIGHT - 8)},
        1.0f,
        UITheme::BORDER
    );
    xPos += UITheme::SPACING_LG;
    
    // Active satellite
    snprintf(buffer, sizeof(buffer), "%s", activeSat.getPreset().name.c_str());
    fonts.drawText(buffer, xPos, yText, UITheme::FONT_SIZE_BODY, 
                   activeSat.getStats().familyColor, true);
    xPos += 80;
    
    // Orbit family badge
    snprintf(buffer, sizeof(buffer), "[%s]", activeSat.getStats().orbitFamily.c_str());
    fonts.drawText(buffer, xPos, yText, UITheme::FONT_SIZE_BODY, 
                   activeSat.getStats().familyColor);
    xPos += 70;
    
    // Separator
    DrawLineEx(
        Vector2{(float)xPos, (float)(yPos + 8)},
        Vector2{(float)xPos, (float)(yPos + UITheme::STATUS_BAR_HEIGHT - 8)},
        1.0f,
        UITheme::BORDER
    );
    xPos += UITheme::SPACING_LG;
    
    // Toggle states
    const char* gridStatus = showGrids ? "Grids: ON" : "Grids: OFF";
    fonts.drawText(gridStatus, xPos, yText, UITheme::FONT_SIZE_BODY, 
                   showGrids ? UITheme::ACCENT : UITheme::TEXT_MUTED);
    xPos += 90;
    
    const char* rotStatus = earthRotation ? "Rot: ON" : "Rot: OFF";
    fonts.drawText(rotStatus, xPos, yText, UITheme::FONT_SIZE_BODY, 
                   earthRotation ? UITheme::ACCENT : UITheme::TEXT_MUTED);
    xPos += 80;
    
    const char* followStatus = cameraFollow ? "Follow: ON" : "Follow: OFF";
    fonts.drawText(followStatus, xPos, yText, UITheme::FONT_SIZE_BODY, 
                   cameraFollow ? UITheme::ACCENT : UITheme::TEXT_MUTED);
    xPos += 110;
    
    // Separator
    DrawLineEx(
        Vector2{(float)xPos, (float)(yPos + 8)},
        Vector2{(float)xPos, (float)(yPos + UITheme::STATUS_BAR_HEIGHT - 8)},
        1.0f,
        UITheme::BORDER
    );
    xPos += UITheme::SPACING_LG;
    
    // FPS
    snprintf(buffer, sizeof(buffer), "FPS: %d", fps);
    Color fpsColor = (fps >= 55) ? UITheme::ACCENT : 
                     (fps >= 30) ? UITheme::WARNING : UITheme::DANGER;
    fonts.drawText(buffer, xPos, yText, UITheme::FONT_SIZE_BODY, fpsColor);
}

void UIManager::drawLeftSidebar(
    const FontSystem &fonts,
    const std::vector<Satellite> &satellites,
    size_t activeSatIndex,
    const Vector3D &sunDirection,
    const std::vector<GroundStation> &groundStations,
    const std::vector<AccessStatistics> &accessStats)
{
    int x = (int)leftSidebarOffset;
    int y = getLeftSidebarY();
    int width = getLeftSidebarWidth();
    int height = getLeftSidebarHeight();
    
    // Handle mouse wheel scrolling when mouse is over sidebar
    Vector2 mousePos = GetMousePosition();
    if (mousePos.x >= x && mousePos.x <= x + width && 
        mousePos.y >= y && mousePos.y <= y + height) {
        float wheel = GetMouseWheelMove();
        leftSidebarScroll -= wheel * 40.0f; // Scroll speed
        
        // Clamp scroll
        float maxScroll = leftSidebarContentHeight - height + 40.0f;
        if (maxScroll < 0) maxScroll = 0;
        if (leftSidebarScroll < 0) leftSidebarScroll = 0;
        if (leftSidebarScroll > maxScroll) leftSidebarScroll = maxScroll;
    }
    
    // Main panel background
    UITheme::DrawPanel(x, y, width, height, UITheme::BORDER_ACCENT);
    
    // Enable scissor mode (clipping) for scrollable content
    BeginScissorMode(x, y, width, height);
    
    int contentX = x + UITheme::PANEL_PADDING;
    int contentY = y + UITheme::PANEL_PADDING - (int)leftSidebarScroll; // Apply scroll
    int contentWidth = width - (UITheme::PANEL_PADDING * 2);
    int yOffset = contentY;
    
    // Draw satellite list
    drawSatelliteList(fonts, satellites, activeSatIndex, contentX, contentY, contentWidth, yOffset);
    
    // Add spacing
    yOffset += UITheme::SPACING_XL;
    
    // Draw solar analysis if enabled
    if (showSolar && activeSatIndex < satellites.size()) {
        drawSolarAnalysis(fonts, satellites[activeSatIndex], sunDirection, 
                         contentX, yOffset, contentWidth, yOffset);
    }
    
    // Add spacing
    yOffset += UITheme::SPACING_XL;
    
    // Draw ground stations if enabled
    if (showGroundStations && !groundStations.empty()) {
        drawGroundStations(fonts, groundStations, contentX, yOffset, contentWidth, yOffset);
        
        yOffset += UITheme::SPACING_XL;
        
        // Draw access windows
        if (activeSatIndex < satellites.size()) {
            drawAccessWindows(fonts, satellites[activeSatIndex], 
                            groundStations, accessStats, 
                            contentX, yOffset, contentWidth, yOffset);
        }
    }
    
    // Store total content height
    leftSidebarContentHeight = (yOffset - contentY) + UITheme::PANEL_PADDING;
    
    EndScissorMode();
    
    // Draw scrollbar if content is scrollable
    float maxScroll = leftSidebarContentHeight - height + 40.0f;
    if (maxScroll > 0) {
        int scrollbarWidth = 6;
        int scrollbarX = x + width - scrollbarWidth - 4;
        int scrollbarY = y + 4;
        int scrollbarHeight = height - 8;
        
        // Scrollbar background
        DrawRectangle(scrollbarX, scrollbarY, scrollbarWidth, scrollbarHeight, 
                     Fade(UITheme::BORDER, 0.3f));
        
        // Scrollbar thumb
        float thumbHeight = (height / leftSidebarContentHeight) * scrollbarHeight;
        if (thumbHeight < 30) thumbHeight = 30; // Minimum thumb size
        float thumbY = scrollbarY + (leftSidebarScroll / maxScroll) * (scrollbarHeight - thumbHeight);
        
        DrawRectangle(scrollbarX, (int)thumbY, scrollbarWidth, (int)thumbHeight, 
                     UITheme::ACCENT);
    }
}


void UIManager::drawSatelliteList(
    const FontSystem &fonts,
    const std::vector<Satellite> &satellites,
    size_t activeSatIndex,
    int x, int y, int width, int& yOffset)
{
    yOffset = y;
    
    // Section header
    fonts.drawText("SATELLITES", x, yOffset, UITheme::FONT_SIZE_H2, UITheme::SECONDARY, true);
    yOffset += 28;
    
    // Divider
    UITheme::DrawDivider(x, yOffset, width);
    yOffset += UITheme::SPACING_MD;
    
    // Satellite list
    for (size_t i = 0; i < satellites.size(); i++) {
        Color textColor = satellites[i].isVisible() ? 
            satellites[i].getStats().familyColor : UITheme::TEXT_MUTED;
        
        const char* activeMarker = (i == activeSatIndex) ? "> " : "  ";
        
        char satText[128];
        snprintf(satText, sizeof(satText), "%s%s", 
                 activeMarker, 
                 satellites[i].getPreset().name.c_str());
        
        fonts.drawText(satText, x, yOffset, UITheme::FONT_SIZE_BODY, textColor, 
                       (i == activeSatIndex));
        
        // Orbit family badge
        snprintf(satText, sizeof(satText), "[%s]", 
                 satellites[i].getStats().orbitFamily.c_str());
        fonts.drawText(satText, x + 140, yOffset, UITheme::FONT_SIZE_SMALL, textColor);
        
        yOffset += 24;
    }
}

void UIManager::drawSolarAnalysis(
    const FontSystem& fonts,
    const Satellite& activeSat,
    const Vector3D& sunDirection,
    int x, int y, int width, int& yOffset)
{
    yOffset = y;
    
    // Section header
    fonts.drawText("GROUND TRACK & SOLAR", x, yOffset, UITheme::FONT_SIZE_H2, 
                   UITheme::WARNING, true);
    yOffset += 28;
    
    // Divider
    UITheme::DrawDivider(x, yOffset, width);
    yOffset += UITheme::SPACING_MD;
    
    // Get subsatellite point (lat/lon)
    GeoCoordinate subsatPoint = GroundTrack::getSubsatellitePoint(
        activeSat.getCurrentState()
    );
    
    // Display lat/lon
    fonts.drawText("Subsatellite Point", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 18;
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Lat:  %+7.2f°", subsatPoint.latitude);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::ACCENT);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "Lon:  %+7.2f°", subsatPoint.longitude);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::ACCENT);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "Alt:  %.1f km", subsatPoint.altitude);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_PRIMARY);
    yOffset += 26;
    
    // Coverage radius
    double coverageRadius = GroundTrack::calculateCoverageRadius(subsatPoint.altitude, 5.0);
    snprintf(buffer, sizeof(buffer), "Coverage: %.0f km radius", coverageRadius);
    fonts.drawText(buffer, x, yOffset, UITheme::FONT_SIZE_SMALL, UITheme::TEXT_MUTED);
    yOffset += 24;
    
    // Divider before solar info
    UITheme::DrawDivider(x, yOffset, width);
    yOffset += UITheme::SPACING_MD;
    
    // Get eclipse and solar data
    EclipseStatus eclipse = EclipseDetector::checkEclipse(
        activeSat.getCurrentState().position,
        sunDirection,
        EARTH_RADIUS
    );
    
    SolarPanelAnalysis solar = SolarAnalyzer::analyze(
        activeSat.getCurrentState().position,
        activeSat.getCurrentState().velocity,
        sunDirection,
        eclipse
    );
    
    // Power status
    fonts.drawText("Power Status", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "%s", solar.getPowerStatus());
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   solar.getEfficiencyColor(), true);
    yOffset += 26;
    
    // Solar efficiency
    fonts.drawText("Solar Efficiency", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "%.1f%%", solar.solarEfficiency * 100.0);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_H3, 
                   solar.getEfficiencyColor(), true);
    yOffset += 26;
    
    // Efficiency bar
    int barWidth = width - UITheme::SPACING_SM;
    int barHeight = 12;
    
    DrawRectangle(x, yOffset, barWidth, barHeight, UITheme::BG_DARK);
    DrawRectangle(x, yOffset, (int)(barWidth * solar.solarEfficiency), barHeight, 
                  solar.getEfficiencyColor());
    DrawRectangleLines(x, yOffset, barWidth, barHeight, UITheme::BORDER);
    yOffset += barHeight + UITheme::SPACING_LG;
    
    // Beta angle
    fonts.drawText("Beta Angle", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "%.2f°", solar.betaAngle);
    Color betaColor = (fabs(solar.betaAngle) < 30.0) ? UITheme::ACCENT : 
                      (fabs(solar.betaAngle) < 60.0) ? UITheme::WARNING : UITheme::DANGER;
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_H3, betaColor);
    yOffset += 26;
    
    // Sun elevation
    fonts.drawText("Sun Elevation", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "%.2f°", solar.sunElevation);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_H3, 
                   UITheme::TEXT_PRIMARY);
    yOffset += 26;
    
    // Design note
    const char* note = "";
    if (fabs(solar.betaAngle) < 30.0) {
        note = "Optimal conditions";
    } else if (fabs(solar.betaAngle) < 60.0) {
        note = "Tracking required";
    } else {
        note = "Thermal challenges";
    }
    
    fonts.drawText(note, x, yOffset, UITheme::FONT_SIZE_SMALL, UITheme::TEXT_MUTED);
}

void UIManager::drawGroundStations(
    const FontSystem& fonts,
    const std::vector<GroundStation>& groundStations,
    int x, int y, int width, int& yOffset)
{
    yOffset = y;
    
    // Section header
    fonts.drawText("GROUND STATIONS", x, yOffset, UITheme::FONT_SIZE_H2, 
                   UITheme::INFO, true);
    yOffset += 28;
    
    // Divider
    UITheme::DrawDivider(x, yOffset, width);
    yOffset += UITheme::SPACING_MD;
    
    // Station list
    for (const auto& station : groundStations) {
        if (!station.visible) continue;
        
        fonts.drawText(station.name.c_str(), x, yOffset, UITheme::FONT_SIZE_BODY, 
                       station.color, true);
        
        // Station code badge
        char codeText[32];
        snprintf(codeText, sizeof(codeText), "[%s]", station.code.c_str());
        fonts.drawText(codeText, x + 140, yOffset, UITheme::FONT_SIZE_SMALL, 
                       station.color);
        yOffset += 20;
        
        // Location
        char locText[128];
        snprintf(locText, sizeof(locText), "  %.1f°N, %.1f°E", 
                 station.location.latitude, station.location.longitude);
        fonts.drawText(locText, x, yOffset, UITheme::FONT_SIZE_SMALL, 
                       UITheme::TEXT_MUTED);
        yOffset += 18;
        
        // Min elevation
        snprintf(locText, sizeof(locText), "  Min Elev: %.0f°", 
                 station.minElevation);
        fonts.drawText(locText, x, yOffset, UITheme::FONT_SIZE_SMALL, 
                       UITheme::TEXT_MUTED);
        yOffset += 24;
    }
}

void UIManager::drawAccessWindows(
    const FontSystem& fonts,
    const Satellite& activeSat,
    const std::vector<GroundStation>& groundStations,
    const std::vector<AccessStatistics>& accessStats,
    int x, int y, int width, int& yOffset)
{
    yOffset = y;
    
    // Section header
    fonts.drawText("ACCESS WINDOWS", x, yOffset, UITheme::FONT_SIZE_H2, 
                   UITheme::SUCCESS, true);
    yOffset += 28;
    
    // Divider
    UITheme::DrawDivider(x, yOffset, width);
    yOffset += UITheme::SPACING_MD;
    
    // Display statistics for each visible station
    for (size_t i = 0; i < groundStations.size() && i < accessStats.size(); ++i) {
        if (!groundStations[i].visible) continue;
        
        const auto& station = groundStations[i];
        const auto& stats = accessStats[i];
        
        // Station name
        fonts.drawText(station.name.c_str(), x, yOffset, UITheme::FONT_SIZE_BODY, 
                       station.color, true);
        yOffset += 20;
        
        char buffer[256];
        
        // Passes per orbit
        snprintf(buffer, sizeof(buffer), "  Passes: %d/orbit", stats.passesPerOrbit);
        fonts.drawText(buffer, x, yOffset, UITheme::FONT_SIZE_SMALL, 
                       UITheme::TEXT_PRIMARY);
        yOffset += 16;
        
        if (stats.passesPerOrbit > 0) {
            // Total access time
            snprintf(buffer, sizeof(buffer), "  Total: %.1f min", 
                     stats.totalAccessTime / 60.0);
            fonts.drawText(buffer, x, yOffset, UITheme::FONT_SIZE_SMALL, 
                           UITheme::TEXT_PRIMARY);
            yOffset += 16;
            
            // Average pass duration
            snprintf(buffer, sizeof(buffer), "  Avg: %.1f min", 
                     stats.averagePassDuration / 60.0);
            fonts.drawText(buffer, x, yOffset, UITheme::FONT_SIZE_SMALL, 
                           UITheme::ACCENT);
            yOffset += 16;
            
            // Longest pass
            snprintf(buffer, sizeof(buffer), "  Max: %.1f min", 
                     stats.longestPass / 60.0);
            fonts.drawText(buffer, x, yOffset, UITheme::FONT_SIZE_SMALL, 
                           UITheme::SUCCESS);
            yOffset += 16;
        } else {
            fonts.drawText("  No access this orbit", x, yOffset, 
                           UITheme::FONT_SIZE_SMALL, UITheme::TEXT_MUTED);
            yOffset += 16;
        }
        
        yOffset += UITheme::SPACING_SM;
    }
    
    if (groundStations.empty()) {
        fonts.drawText("No ground stations", x, yOffset, UITheme::FONT_SIZE_SMALL, 
                       UITheme::TEXT_MUTED);
    }
}

void UIManager::drawRightSidebar(
    const FontSystem &fonts,
    const Satellite &activeSat,
    const OrbitalElements &elements,
    const Vector3D &sunDirection)
{
    int x = getRightSidebarX() + (int)rightSidebarOffset;
    int y = getRightSidebarY();
    int width = getRightSidebarWidth();
    int height = getRightSidebarHeight();
    
    // Main panel background
    UITheme::DrawPanel(x, y, width, height, activeSat.getStats().familyColor);
    
    int contentX = x + UITheme::PANEL_PADDING;
    int contentY = y + UITheme::PANEL_PADDING;
    
    drawOrbitalElements(fonts, activeSat, elements, sunDirection, contentX, contentY, 
                        width - (UITheme::PANEL_PADDING * 2));
}

void UIManager::drawOrbitalElements(
    const FontSystem &fonts,
    const Satellite &activeSat,
    const OrbitalElements &elements,
    const Vector3D &sunDirection,
    int x, int y, int width)
{
    int yOffset = y;
    char buffer[256];
    
    // Header
    fonts.drawText("ORBITAL ELEMENTS", x, yOffset, UITheme::FONT_SIZE_H1, 
                   activeSat.getStats().familyColor, true);
    yOffset += 30;
    
    // Description
    fonts.drawText(activeSat.getPreset().description.c_str(), x, yOffset, 
                   UITheme::FONT_SIZE_SMALL, UITheme::TEXT_SECONDARY);
    yOffset += 22;
    
    // Orbit family badge
    int badgeWidth = 90;
    int badgeHeight = 26;
    DrawRectangle(x, yOffset, badgeWidth, badgeHeight, activeSat.getStats().familyColor);
    fonts.drawText(activeSat.getStats().orbitFamily.c_str(), 
                   x + 10, yOffset + 5, UITheme::FONT_SIZE_BODY, BLACK, true);
    yOffset += badgeHeight + UITheme::SPACING_LG;
    
    // Orbit type
    snprintf(buffer, sizeof(buffer), "Type: %s", elements.orbitType().c_str());
    fonts.drawText(buffer, x, yOffset, UITheme::FONT_SIZE_BODY, UITheme::TEXT_PRIMARY);
    yOffset += 24;
    
    // Eclipse status (if enabled)
    if (showEclipse) {
        EclipseStatus eclipse = EclipseDetector::checkEclipse(
            activeSat.getCurrentState().position,
            sunDirection,
            EARTH_RADIUS
        );
        
        const char* eclipseText = "Sunlit";
        Color eclipseColor = UITheme::ACCENT;
        
        if (eclipse.inUmbra) {
            eclipseText = "UMBRA";
            eclipseColor = UITheme::DANGER;
        } else if (eclipse.inPenumbra) {
            eclipseText = "PENUMBRA";
            eclipseColor = UITheme::WARNING;
        }
        
        fonts.drawText("Eclipse:", x, yOffset, UITheme::FONT_SIZE_BODY, 
                       UITheme::TEXT_SECONDARY);
        snprintf(buffer, sizeof(buffer), "%s", eclipseText);
        fonts.drawText(buffer, x + 80, yOffset, UITheme::FONT_SIZE_BODY, eclipseColor, true);
        yOffset += 24;
    }
    
    yOffset += UITheme::SPACING_SM;
    UITheme::DrawDivider(x, yOffset, width);
    yOffset += UITheme::SPACING_LG;
    
    // ORBIT STATISTICS
    fonts.drawText("ORBIT STATISTICS", x, yOffset, UITheme::FONT_SIZE_H2, 
                   UITheme::WARNING, true);
    yOffset += 24;
    
    // Altitude Range
    fonts.drawText("Altitude Range", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "Periapsis: %.1f km", activeSat.getStats().periapsisAlt);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::WARNING);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "Apoapsis:  %.1f km", activeSat.getStats().apoapsisAlt);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   Color{200, 100, 255, 255}); // Purple
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "Mean:      %.1f km", activeSat.getStats().meanAltitude);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_PRIMARY);
    yOffset += 24;
    
    // Velocity Range
    fonts.drawText("Velocity Range", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "At Periapsis: %.2f km/s", activeSat.getStats().periapsisVel);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::WARNING);
    yOffset += 18;
    
    snprintf(buffer, sizeof(buffer), "At Apoapsis:  %.2f km/s", activeSat.getStats().apoapsisVel);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   Color{200, 100, 255, 255});
    yOffset += 18;
    
    double currentVel = activeSat.getCurrentState().velocity.magnitude();
    snprintf(buffer, sizeof(buffer), "Current:      %.2f km/s", currentVel);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::ACCENT);
    yOffset += 24;
    
    yOffset += UITheme::SPACING_SM;
    UITheme::DrawDivider(x, yOffset, width);
    yOffset += UITheme::SPACING_LG;
    
    // CLASSICAL ELEMENTS
    fonts.drawText("CLASSICAL ELEMENTS", x, yOffset, UITheme::FONT_SIZE_H2, 
                   UITheme::WARNING, true);
    yOffset += 24;
    
    // Semi-major axis
    fonts.drawText("Semi-major axis (a)", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 16;
    snprintf(buffer, sizeof(buffer), "%.2f km", elements.semiMajorAxis);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_PRIMARY);
    yOffset += 22;
    
    // Eccentricity
    fonts.drawText("Eccentricity (e)", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 16;
    snprintf(buffer, sizeof(buffer), "%.6f", elements.eccentricity);
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_PRIMARY);
    yOffset += 22;
    
    // Inclination
    fonts.drawText("Inclination (i)", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 16;
    snprintf(buffer, sizeof(buffer), "%.2f°", elements.inclinationDeg());
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_PRIMARY);
    yOffset += 22;
    
    // RAAN
    fonts.drawText("RAAN (Ω)", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 16;
    snprintf(buffer, sizeof(buffer), "%.2f°", elements.raanDeg());
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_PRIMARY);
    yOffset += 22;
    
    // Argument of Periapsis
    fonts.drawText("Arg. Periapsis (ω)", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 16;
    snprintf(buffer, sizeof(buffer), "%.2f°", elements.argumentOfPeriapsisDeg());
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_PRIMARY);
    yOffset += 22;
    
    // True Anomaly
    fonts.drawText("True Anomaly (ν)", x, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_SECONDARY);
    yOffset += 16;
    snprintf(buffer, sizeof(buffer), "%.2f°", elements.trueAnomalyDeg());
    fonts.drawText(buffer, x + UITheme::SPACING_SM, yOffset, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_PRIMARY);
    yOffset += 22;
    
    // Period
    yOffset += UITheme::SPACING_SM;
    snprintf(buffer, sizeof(buffer), "Period: %.2f min", elements.period / 60.0);
    fonts.drawText(buffer, x, yOffset, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
}

void UIManager::drawKeyboardLegend(const FontSystem &fonts)
{
    // Smaller help panel - positioned in center but more compact
    int panelW = 580;
    int panelH = 540;
    int panelX = (screenWidth - panelW) / 2;
    int panelY = (screenHeight - panelH) / 2;
    
    // Semi-transparent dark overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
    
    // Help panel
    UITheme::DrawPanel(panelX, panelY, panelW, panelH, UITheme::SECONDARY);
    
    int x = panelX + UITheme::SPACING_XL;
    int y = panelY + UITheme::SPACING_LG;
    
    fonts.drawText("KEYBOARD SHORTCUTS", x + 130, y, UITheme::FONT_SIZE_H1, 
                   UITheme::SECONDARY, true);
    y += 40;
    
    UITheme::DrawDivider(panelX + UITheme::SPACING_LG, y, panelW - UITheme::SPACING_XL);
    y += UITheme::SPACING_LG;
    
    int col1X = x;
    int col2X = x + 270;
    
    // Column 1
    fonts.drawText("SIMULATION", col1X, y, UITheme::FONT_SIZE_H3, UITheme::WARNING, true);
    y += 22;
    fonts.drawText("SPACE", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Pause/Resume", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("↑ / ↓", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Speed Up/Down", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("R", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Earth Rotation", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 28;
    
    fonts.drawText("CAMERA", col1X, y, UITheme::FONT_SIZE_H3, UITheme::WARNING, true);
    y += 22;
    fonts.drawText("RMB", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Rotate View", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("WHEEL", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Zoom In/Out", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("F", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Follow Mode", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("1/2/3/4", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Presets", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 28;
    
    fonts.drawText("ORBITS", col1X, y, UITheme::FONT_SIZE_H3, UITheme::WARNING, true);
    y += 22;
    fonts.drawText("TAB", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Cycle Active", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("Q-Z", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Toggle 1-10", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("CTRL+V", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Show All", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("CTRL+B", col1X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Hide All", col1X + 80, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    
    // Column 2
    y = panelY + UITheme::SPACING_LG + 40 + UITheme::SPACING_LG;
    
    fonts.drawText("DISPLAY", col2X, y, UITheme::FONT_SIZE_H3, UITheme::WARNING, true);
    y += 22;
    fonts.drawText("C", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Left Sidebar", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("E", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Right Sidebar", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("G", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Toggle Grids", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("V", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Eclipse Display", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("Y", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Solar Analysis", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("T", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("Ground Stations", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("X", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ACCENT, true);
    fonts.drawText("This Help", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 28;
    
    fonts.drawText("ORBIT FAMILIES", col2X, y, UITheme::FONT_SIZE_H3, UITheme::WARNING, true);
    y += 22;
    fonts.drawText("LEO", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ORBIT_LEO, true);
    fonts.drawText("< 2,000 km", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("MEO", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ORBIT_MEO, true);
    fonts.drawText("2k - 35k km", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("HEO", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ORBIT_HEO, true);
    fonts.drawText("Elliptical", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    y += 20;
    fonts.drawText("GEO", col2X, y, UITheme::FONT_SIZE_BODY, UITheme::ORBIT_GEO, true);
    fonts.drawText("~36k km", col2X + 60, y, UITheme::FONT_SIZE_BODY, UITheme::TEXT_SECONDARY);
    
    // Close instruction
    y = panelY + panelH - 40;
    fonts.drawText("Press X to close", panelX + 220, y, UITheme::FONT_SIZE_BODY, 
                   UITheme::TEXT_MUTED);
}

bool UIManager::isMouseOverUI() const {
    Vector2 mousePos = GetMousePosition();
    
    // Check if mouse is over help overlay (full screen when active)
    if (showHelp) {
        return true;
    }
    
    // Check if mouse is over title bar
    if (mousePos.y <= UITheme::TITLE_BAR_HEIGHT) {
        return true;
    }
    
    // Check if mouse is over status bar
    if (mousePos.y >= screenHeight - UITheme::STATUS_BAR_HEIGHT) {
        return true;
    }
    
    // Check if mouse is over left sidebar (when visible or animating)
    if (leftSidebarOffset > -(float)UITheme::SIDEBAR_WIDTH + 10.0f) {
        int leftX = (int)leftSidebarOffset;
        int leftWidth = UITheme::SIDEBAR_WIDTH;
        int leftY = UITheme::TITLE_BAR_HEIGHT;
        int leftHeight = screenHeight - UITheme::TITLE_BAR_HEIGHT - UITheme::STATUS_BAR_HEIGHT;
        
        if (mousePos.x >= leftX && mousePos.x <= leftX + leftWidth &&
            mousePos.y >= leftY && mousePos.y <= leftY + leftHeight) {
            return true;
        }
    }
    
    // Check if mouse is over right sidebar (when visible or animating)
    if (rightSidebarOffset < (float)UITheme::SIDEBAR_WIDTH - 10.0f) {
        int rightX = screenWidth - UITheme::SIDEBAR_WIDTH + (int)rightSidebarOffset;
        int rightWidth = UITheme::SIDEBAR_WIDTH;
        int rightY = UITheme::TITLE_BAR_HEIGHT;
        int rightHeight = screenHeight - UITheme::TITLE_BAR_HEIGHT - UITheme::STATUS_BAR_HEIGHT;
        
        if (mousePos.x >= rightX && mousePos.x <= rightX + rightWidth &&
            mousePos.y >= rightY && mousePos.y <= rightY + rightHeight) {
            return true;
        }
    }
    
    return false;
}
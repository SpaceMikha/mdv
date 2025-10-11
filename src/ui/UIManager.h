#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "FontSystem.h"
#include "Satellite.h"
#include "OrbitalElements.h"
#include "Eclipse.h"
#include "SolarAnalysis.h"
#include "Vector3D.h"
#include "UITheme.h"
#include "GroundStation.h" 
#include <vector>

// UI Manager - handles all UI rendering with organized layout
class UIManager {
public:
    UIManager(int screenWidth, int screenHeight);
     
    // Update animations
    void update(float deltaTime);
    
    // Draw all UI elements
    void draw(
        const FontSystem& fonts,
        const std::vector<Satellite>& satellites,
        size_t activeSatIndex,
        const OrbitalElements& currentElements,
        float animationSpeed,
        bool showGrids,
        bool earthRotation,
        bool cameraFollow,
        int fps,
        const Vector3D& sunDirection,
        const std::vector<GroundStation>& groundStations,        
        const std::vector<AccessStatistics>& accessStats        
    );
    
    // Toggle panels
    void toggleLeftSidebar() { showLeftSidebar = !showLeftSidebar; }
    void toggleRightSidebar() { showRightSidebar = !showRightSidebar; }
    void toggleHelp() { showHelp = !showHelp; }
    
    // Legacy toggles (for backward compatibility with existing controls)
    void toggleElements() { showRightSidebar = !showRightSidebar; }
    void toggleList() { showLeftSidebar = !showLeftSidebar; }
    void toggleEclipse() { showEclipse = !showEclipse; }
    void toggleSolar() { showSolar = !showSolar; }
    void toggleGroundStations() { showGroundStations = !showGroundStations; } 
    
    // Getters
    bool isShowingLeftSidebar() const { return showLeftSidebar; }
    bool isShowingRightSidebar() const { return showRightSidebar; }
    bool isShowingHelp() const { return showHelp; }
    bool isShowingEclipse() const { return showEclipse; }
    bool isShowingSolar() const { return showSolar; }
    bool isShowingGroundStations() const { return showGroundStations; } 
    
    // Legacy getters
    bool isShowingElements() const { return showRightSidebar; }
    bool isShowingList() const { return showLeftSidebar; }
    
    // Check if mouse is over UI
    bool isMouseOverUI() const;
    
private:
    int screenWidth;
    int screenHeight;
    
    // Sidebar visibility
    bool showLeftSidebar;
    bool showRightSidebar;
    bool showHelp;
    
    // Feature toggles
    bool showEclipse;
    bool showSolar;
    bool showGroundStations; 
    
    // Animation state
    float leftSidebarOffset;
    float rightSidebarOffset;
    float targetLeftOffset;
    float targetRightOffset;
    
    // Scrolling state
    float leftSidebarScroll;
    float leftSidebarContentHeight;
    
    // Layout calculations
    int getLeftSidebarX() const { return 0; }
    int getLeftSidebarY() const { return UITheme::TITLE_BAR_HEIGHT; }
    int getLeftSidebarWidth() const { return UITheme::SIDEBAR_WIDTH; }
    int getLeftSidebarHeight() const { 
        return screenHeight - UITheme::TITLE_BAR_HEIGHT - UITheme::STATUS_BAR_HEIGHT; 
    }
    
    int getRightSidebarX() const { return screenWidth - UITheme::SIDEBAR_WIDTH; }
    int getRightSidebarY() const { return UITheme::TITLE_BAR_HEIGHT; }
    int getRightSidebarWidth() const { return UITheme::SIDEBAR_WIDTH; }
    int getRightSidebarHeight() const { 
        return screenHeight - UITheme::TITLE_BAR_HEIGHT - UITheme::STATUS_BAR_HEIGHT; 
    }
    
    // Draw individual components
    void drawTitleBar(const FontSystem& fonts);
    void drawStatusBar(
        const FontSystem& fonts,
        const Satellite& activeSat,
        float animationSpeed,
        bool showGrids,
        bool earthRotation,
        bool cameraFollow,
        int fps
    );
    
    // Left sidebar components
    void drawLeftSidebar(
        const FontSystem& fonts,
        const std::vector<Satellite>& satellites,
        size_t activeSatIndex,
        const Vector3D& sunDirection,
        const std::vector<GroundStation>& groundStations,        
        const std::vector<AccessStatistics>& accessStats         
    );
    void drawSatelliteList(
        const FontSystem& fonts,
        const std::vector<Satellite>& satellites,
        size_t activeSatIndex,
        int x, int y, int width, int& yOffset
    );
    void drawSolarAnalysis(
        const FontSystem& fonts,
        const Satellite& activeSat,
        const Vector3D& sunDirection,
        int x, int y, int width, int& yOffset
    );
    
    // Ground station components 
    void drawGroundStations(
        const FontSystem& fonts,
        const std::vector<GroundStation>& groundStations,
        int x, int y, int width, int& yOffset
    );
    
    void drawAccessWindows(
        const FontSystem& fonts,
        const Satellite& activeSat,
        const std::vector<GroundStation>& groundStations,
        const std::vector<AccessStatistics>& accessStats,
        int x, int y, int width, int& yOffset
    );
    
    // Right sidebar components
    void drawRightSidebar(
        const FontSystem& fonts,
        const Satellite& activeSat,
        const OrbitalElements& elements,
        const Vector3D& sunDirection
    );
    void drawOrbitalElements(
        const FontSystem& fonts,
        const Satellite& activeSat,
        const OrbitalElements& elements,
        const Vector3D& sunDirection,
        int x, int y, int width
    );
    
    // Help overlay
    void drawKeyboardLegend(const FontSystem& fonts);
};

#endif // UI_MANAGER_H
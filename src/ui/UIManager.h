#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "FontSystem.h"
#include "Satellite.h"
#include "OrbitalElements.h"
#include "Eclipse.h"
#include "SolarAnalysis.h"  
#include "Vector3D.h"
#include <vector>

// UI Manager - handles all UI rendering
class UIManager {
public:
    UIManager(int screenWidth, int screenHeight);
    
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
        const Vector3D& sunDirection
    );
    
    // Toggle panels
    void toggleElements() { showElements = !showElements; }
    void toggleList() { showList = !showList; }
    void toggleHelp() { showHelp = !showHelp; }
    void toggleEclipse() { showEclipse = !showEclipse; }
    void toggleSolar() { showSolar = !showSolar; } 
    
    // Getters
    bool isShowingElements() const { return showElements; }
    bool isShowingList() const { return showList; }
    bool isShowingHelp() const { return showHelp; }
    bool isShowingEclipse() const { return showEclipse; }
    bool isShowingSolar() const { return showSolar; }  
    
private:
    int screenWidth;
    int screenHeight;
    bool showElements;
    bool showList;
    bool showHelp;
    bool showEclipse;
    bool showSolar;  
    
    // Draw individual panels
    void drawTitle(const FontSystem& fonts);
    void drawStatusBar(
        const FontSystem& fonts,
        const Satellite& activeSat,
        float animationSpeed,
        bool showGrids,
        bool earthRotation,
        bool cameraFollow,
        int fps
    );
    void drawSatelliteList(
        const FontSystem& fonts,
        const std::vector<Satellite>& satellites,
        size_t activeSatIndex
    );
    void drawOrbitalElements(
        const FontSystem& fonts,
        const Satellite& activeSat,
        const OrbitalElements& elements,
        const Vector3D& sunDirection
    );
    void drawKeyboardLegend(const FontSystem& fonts);
    
    
    void drawSolarAnalysis(
        const FontSystem& fonts,
        const Satellite& activeSat,
        const Vector3D& sunDirection
    );
};

#endif // UI_MANAGER_H
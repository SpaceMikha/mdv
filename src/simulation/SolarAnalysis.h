#ifndef SOLAR_ANALYSIS_H
#define SOLAR_ANALYSIS_H

#include "Vector3D.h"
#include "Eclipse.h"
#include "raylib.h"

// Solar panel analysis structure
struct SolarPanelAnalysis {
    double betaAngle;           // Angle between sun and orbital plane (degrees)
    double sunElevation;        // Elevation angle of sun from orbital plane
    double solarEfficiency;     // 0.0 to 1.0 (assumes tracking panels)
    Vector3D sunVector;         // Unit vector from satellite to sun
    bool inSunlight;           // True if not in eclipse
    
    SolarPanelAnalysis();
    
    // Get color based on efficiency
    Color getEfficiencyColor() const;
    
    // Get power status string
    const char* getPowerStatus() const;
};

// Solar analysis class
class SolarAnalyzer {
public:
    // Analyze solar conditions for a satellite
    static SolarPanelAnalysis analyze(
        const Vector3D& satPos,
        const Vector3D& satVel,
        const Vector3D& sunDir,
        const EclipseStatus& eclipse
    );
};

#endif // SOLAR_ANALYSIS_H
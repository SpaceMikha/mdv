#include "SolarAnalysis.h"
#include "Constants.h"
#include <cmath>

SolarPanelAnalysis::SolarPanelAnalysis()
    : betaAngle(0), sunElevation(0), solarEfficiency(0),
      sunVector(0, 0, 0), inSunlight(true) {}

Color SolarPanelAnalysis::getEfficiencyColor() const {
    if (solarEfficiency > 0.8) return GREEN;
    if (solarEfficiency > 0.5) return YELLOW;
    if (solarEfficiency > 0.2) return ORANGE;
    return RED;
}

const char* SolarPanelAnalysis::getPowerStatus() const {
    if (!inSunlight) return "Battery Mode";
    if (solarEfficiency > 0.8) return "Optimal Generation";
    if (solarEfficiency > 0.5) return "Good Generation";
    return "Limited Generation";
}

SolarPanelAnalysis SolarAnalyzer::analyze(
    const Vector3D& satPos,
    const Vector3D& satVel,
    const Vector3D& sunDir,
    const EclipseStatus& eclipse
) {
    SolarPanelAnalysis analysis;
    
    // Sun vector from satellite (normalized)
    Vector3D sunNorm = sunDir.normalized();
    analysis.sunVector = sunNorm;
    
    // Not in sunlight if in umbra
    analysis.inSunlight = !eclipse.inUmbra;
    
    if (eclipse.inUmbra) {
        // In full shadow - no solar power
        analysis.solarEfficiency = 0.0;
        analysis.betaAngle = 0.0;
        analysis.sunElevation = 0.0;
        return analysis;
    }
    
    // Calculate orbital plane normal (angular momentum direction)
    Vector3D orbitalNormal = satPos.cross(satVel).normalized();
    
    // Beta angle: angle between sun vector and orbital plane
    // Beta = 90° - angle between sun and orbit normal
    double sunNormalDot = sunNorm.dot(orbitalNormal);
    analysis.sunElevation = asin(sunNormalDot) * 180.0 / M_PI;
    analysis.betaAngle = analysis.sunElevation;
    
    // Solar efficiency calculation (tracking panels)
    if (eclipse.inPenumbra) {
        // Partial shadow - reduced efficiency
        analysis.solarEfficiency = SOLAR_EFFICIENCY_PENUMBRA * 
                                   fabs(cos(analysis.betaAngle * M_PI / 180.0));
    } else {
        // Full sunlight - efficiency based on beta angle
        analysis.solarEfficiency = fabs(cos(analysis.betaAngle * M_PI / 180.0));
        
        // Apply realistic constraints at extreme beta angles
        if (fabs(analysis.betaAngle) > HIGH_BETA_THRESHOLD) {
            analysis.solarEfficiency *= SOLAR_EFFICIENCY_HIGH_BETA;
        }
    }
    
    // Clamp to [0, 1]
    if (analysis.solarEfficiency < 0.0) analysis.solarEfficiency = 0.0;
    if (analysis.solarEfficiency > 1.0) analysis.solarEfficiency = 1.0;
    
    return analysis;
}
#ifndef ORBIT_RENDERER_H
#define ORBIT_RENDERER_H

#include "Satellite.h"
#include "Eclipse.h"
#include "Vector3D.h"
#include "raylib.h"
#include <vector>

// Orbit and satellite rendering
class OrbitRenderer {
public:
    // Draw all visible satellites
    static void drawSatellites(
        const std::vector<Satellite>& satellites,
        size_t activeSatelliteIndex,
        const Vector3D& sunDirection,
        bool showEclipse
    );
    
    // Draw a single satellite
    static void drawSatellite(
        const Satellite& sat,
        bool isActive,
        const Vector3D& sunDirection,
        bool showEclipse
    );
    
    // Draw orbit line
    static void drawOrbitLine(
        const Satellite& sat,
        bool isActive
    );
    
    // Draw satellite trail
    static void drawTrail(
        const Satellite& sat
    );
    
    // Draw velocity vector
    static void drawVelocityVector(
        const Satellite& sat,
        float length = 2000.0f
    );
    
    // Draw periapsis and apoapsis markers
    static void drawApsisMarkers(
        const Satellite& sat,
        double mu
    );
};

#endif // ORBIT_RENDERER_H
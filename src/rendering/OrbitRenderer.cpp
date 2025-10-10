#include "OrbitRenderer.h"
#include "RenderUtils.h"
#include "Constants.h"
#include "Eclipse.h"
#include "OrbitalElements.h"

void OrbitRenderer::drawSatellites(
    const std::vector<Satellite>& satellites,
    size_t activeSatelliteIndex,
    const Vector3D& sunDirection,
    bool showEclipse
) {
    for (size_t i = 0; i < satellites.size(); i++) {
        if (!satellites[i].isVisible()) continue;
        
        bool isActive = (i == activeSatelliteIndex);
        
        // Draw orbit line
        drawOrbitLine(satellites[i], isActive);
        
        // Draw satellite
        drawSatellite(satellites[i], isActive, sunDirection, showEclipse);
        
        // Draw trail
        drawTrail(satellites[i]);
        
        // Draw velocity vector (only for active)
        if (isActive) {
            drawVelocityVector(satellites[i]);
            drawApsisMarkers(satellites[i], MU_EARTH);
        }
    }
}

void OrbitRenderer::drawSatellite(
    const Satellite& sat,
    bool isActive,
    const Vector3D& sunDirection,
    bool showEclipse
) {
    Vector3 scPos = RenderUtils::toRaylib(sat.getCurrentState().position);
    float satSize = isActive ? 0.4f : 0.25f;
    Color orbitColor = sat.getPreset().color;
    
    // Check eclipse status
    Color satColor = orbitColor;
    if (showEclipse) {
        EclipseStatus eclipse = EclipseDetector::checkEclipse(
            sat.getCurrentState().position,
            sunDirection,
            EARTH_RADIUS
        );

        if (eclipse.inUmbra) {
            // Full shadow - darken significantly
            satColor = Color{
                (unsigned char)(orbitColor.r * 0.2),
                (unsigned char)(orbitColor.g * 0.2),
                (unsigned char)(orbitColor.b * 0.2),
                orbitColor.a
            };
        } else if (eclipse.inPenumbra) {
            // Partial shadow - darken moderately
            satColor = Color{
                (unsigned char)(orbitColor.r * 0.5),
                (unsigned char)(orbitColor.g * 0.5),
                (unsigned char)(orbitColor.b * 0.5),
                orbitColor.a
            };
        }
    }

    DrawSphere(scPos, satSize, satColor);
}

void OrbitRenderer::drawOrbitLine(
    const Satellite& sat,
    bool isActive
) {
    Color lineColor = isActive ? 
        sat.getStats().familyColor : 
        Fade(sat.getStats().familyColor, 0.4f);
    
    for (size_t i = 1; i < sat.getOrbit().size(); i++) {
        Vector3 p1 = RenderUtils::toRaylib(sat.getOrbit()[i-1].position);
        Vector3 p2 = RenderUtils::toRaylib(sat.getOrbit()[i].position);
        DrawLine3D(p1, p2, lineColor);
    }
}

void OrbitRenderer::drawTrail(const Satellite& sat) {
    if (sat.getCurrentFrame() <= TRAIL_LENGTH) return;
    
    Color orbitColor = sat.getPreset().color;
    
    for (size_t i = sat.getCurrentFrame() - TRAIL_LENGTH; i < sat.getCurrentFrame(); i++) {
        float alpha = (float)(i - (sat.getCurrentFrame() - TRAIL_LENGTH)) / TRAIL_LENGTH;
        Vector3 trailPos = RenderUtils::toRaylib(sat.getOrbit()[i].position);
        DrawSphere(trailPos, 0.1f, Fade(orbitColor, alpha * 0.5f));
    }
}

void OrbitRenderer::drawVelocityVector(const Satellite& sat, float length) {
    Vector3 scPos = RenderUtils::toRaylib(sat.getCurrentState().position);
    Vector3D velScaled = sat.getCurrentState().velocity.normalized() * length;
    Vector3 velEnd = RenderUtils::toRaylib(sat.getCurrentState().position + velScaled);
    DrawLine3D(scPos, velEnd, GREEN);
}

void OrbitRenderer::drawApsisMarkers(const Satellite& sat, double mu) {
    OrbitalElements elements = OrbitalElements::fromStateVector(
        sat.getOrbit()[0], 
        mu
    );
    
    if (elements.eccentricity > 0.01) {
        // Periapsis marker
        size_t periIdx = 0;
        DrawSphere(
            RenderUtils::toRaylib(sat.getOrbit()[periIdx].position), 
            0.3f, 
            ORANGE
        );
        
        // Apoapsis marker
        size_t apoIdx = sat.getOrbit().size() / 2;
        DrawSphere(
            RenderUtils::toRaylib(sat.getOrbit()[apoIdx].position), 
            0.3f, 
            PURPLE
        );
        
        // Line between apsis points
        DrawLine3D(
            RenderUtils::toRaylib(sat.getOrbit()[periIdx].position),
            RenderUtils::toRaylib(sat.getOrbit()[apoIdx].position),
            Fade(WHITE, 0.3f)
        );
    }
}
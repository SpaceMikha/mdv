#include "GroundStationRenderer.h"
#include "RenderUtils.h"
#include "Constants.h"
#include "GroundTrack.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void GroundStationRenderer::drawGroundStation(
    const GroundStation& station,
    double earthRadius
) {
    if (!station.visible) return;
    
    Vector3 pos = latLonToSurfacePosition(
        station.location.latitude,
        station.location.longitude,
        earthRadius
    );
    
    // Scale to render coordinates
    pos.x *= SCALE;
    pos.y *= SCALE;
    pos.z *= SCALE;
    
    // Offset slightly above surface
    float offset = 1.005f;  
    pos.x *= offset;
    pos.y *= offset;
    pos.z *= offset;
    
    // Draw station marker 
    DrawSphere(pos, 0.15f, station.color);  
    // Draw station antenna (small vertical line)
    Vector3 antennaTop = {
        pos.x * 1.02f, 
        pos.y * 1.02f,
        pos.z * 1.02f
    };
    DrawLine3D(pos, antennaTop, Fade(station.color, 0.8f));
    
    // Draw small circle around station 
    DrawCircle3D(pos, 0.2f,  
                 Vector3{pos.x, pos.y, pos.z}, 
                 0.0f, 
                 Fade(station.color, 0.5f));
    
}

void GroundStationRenderer::drawCommunicationCone(
    const GroundStation& station,
    double earthRadius,
    int segments
) {
    if (!station.visible) return;
    
    // Calculate horizon distance based on minimum elevation angle
    double minElevRad = station.minElevation * M_PI / 180.0;
    
    // Angular radius of visibility cone
    double earthCentralAngle = acos(
        earthRadius / (earthRadius + station.location.altitude) * cos(minElevRad)
    ) - minElevRad;
    
    // Radius on Earth's surface
    double radiusKm = earthRadius * earthCentralAngle;
    
    // Draw the visibility circle
    drawSurfaceCircle(
        station.location.latitude,
        station.location.longitude,
        radiusKm,
        Fade(station.color, 0.2f),
        segments,
        earthRadius
    );
}

void GroundStationRenderer::drawLineOfSight(
    const GroundStation& station,
    const Satellite& satellite,
    double earthRadius,
    double currentTime
) {
    if (!station.visible || !satellite.isVisible()) return;
    
    // Check if satellite is visible
    bool visible = GroundStationAccess::isVisible(
        satellite.getCurrentState().position,
        station,
        earthRadius,
        currentTime
    );
    
    if (!visible) return;
    
    // Station position in ECI
    Vector3D stationECI = GroundTrack::LatLonToECI(station.location, currentTime);
    Vector3 stationPos = RenderUtils::toRaylib(stationECI);
    
    // Satellite position
    Vector3 satPos = RenderUtils::toRaylib(satellite.getCurrentState().position);
    
    // Draw line of sight
    Color lineColor = Fade(station.color, 0.6f);
    DrawLine3D(stationPos, satPos, lineColor);
    
    // Draw small marker at midpoint
    Vector3 midPoint = {
        (stationPos.x + satPos.x) * 0.5f,
        (stationPos.y + satPos.y) * 0.5f,
        (stationPos.z + satPos.z) * 0.5f
    };
    DrawSphere(midPoint, 0.1f, Fade(lineColor, 0.8f));
}

void GroundStationRenderer::drawAllGroundStations(
    const std::vector<GroundStation>& stations,
    bool showCones,
    double earthRadius
) {
    for (const auto& station : stations) {
        if (!station.visible) continue;
        
        if (showCones) {
            drawCommunicationCone(station, earthRadius);
        }
        
        drawGroundStation(station, earthRadius);
    }
}

void GroundStationRenderer::drawAccessVisualization(
    const GroundStation& station,
    const Satellite& satellite,
    const AccessStatistics& stats,
    double earthRadius,
    double currentTime
) {
    if (!station.visible || !satellite.isVisible()) return;
    
    // Check current visibility
    bool inAccess = GroundStationAccess::isVisible(
        satellite.getCurrentState().position,
        station,
        earthRadius,
        currentTime
    );
    
    if (inAccess) {
        // Draw bright line of sight when in access
        drawLineOfSight(station, satellite, earthRadius, currentTime);
        
        // Highlight station with pulsing effect
        Vector3 pos = latLonToSurfacePosition(
            station.location.latitude,
            station.location.longitude,
            earthRadius
        );
        pos.x *= SCALE * 1.01f;
        pos.y *= SCALE * 1.01f;
        pos.z *= SCALE * 1.01f;
        
        // Pulsing ring effect
        float pulseSize = 0.5f + 0.2f * sinf((float)currentTime * 3.0f);
        DrawCircle3D(pos, pulseSize, 
                     Vector3{pos.x, pos.y, pos.z}, 
                     0.0f, 
                     Fade(station.color, 0.6f));
    }
}

Vector3 GroundStationRenderer::latLonToSurfacePosition(
    double latitude,
    double longitude,
    double earthRadius
) {
    double latRad = latitude * M_PI / 180.0;
    double lonRad = longitude * M_PI / 180.0;
    
    // Convert to Cartesian coordinates
    // Note: This is in a fixed frame that matches Earth's texture orientation
    // The Earth model itself rotates, and stations are drawn in the same frame,
    // so they will rotate with the Earth automatically
    float x = earthRadius * cos(latRad) * cos(lonRad);
    float y = earthRadius * cos(latRad) * sin(lonRad);
    float z = earthRadius * sin(latRad);
    
    return Vector3{x, y, z};
}

void GroundStationRenderer::drawSurfaceCircle(
    double centerLat,
    double centerLon,
    double radiusKm,
    Color color,
    int segments,
    double earthRadius
) {
    // Angular radius in radians
    double angularRadius = radiusKm / earthRadius;
    
    // Create circle points
    std::vector<Vector3> circlePoints;
    
    for (int i = 0; i <= segments; i++) {
        double theta = (2.0 * M_PI * i) / segments;
        
        // Calculate point on circle using spherical geometry
        double latRad = centerLat * M_PI / 180.0;
        double lonRad = centerLon * M_PI / 180.0;
        
        // Simple approximation for small circles
        double pointLat = latRad + angularRadius * sin(theta);
        double pointLon = lonRad + angularRadius * cos(theta) / cos(latRad);
        
        // Convert to surface position
        double x = earthRadius * cos(pointLat) * cos(pointLon);
        double y = earthRadius * cos(pointLat) * sin(pointLon);
        double z = earthRadius * sin(pointLat);
        
        // Scale and offset
        float offset = 1.007f;
        circlePoints.push_back(Vector3{
            (float)(x * SCALE * offset),
            (float)(y * SCALE * offset),
            (float)(z * SCALE * offset)
        });
    }
    
    // Draw circle segments
    for (size_t i = 1; i < circlePoints.size(); i++) {
        DrawLine3D(circlePoints[i - 1], circlePoints[i], color);
    }
}
#include "GroundTrackRenderer.h"
#include "Constants.h"
#include "RenderUtils.h"
#include <cmath>

void GroundTrackRenderer::drawGroundTrack(
    const Satellite& sat,
    Color trackColor,
    bool isActive
) {
    if (!sat.isVisible()) return;
    
    // Calculate ground track for the entire orbit
    std::vector<GeoCoordinate> groundTrack = GroundTrack::calculateGroundTrack(
        sat.getOrbit(),
        360  // Sample every degree
    );
    
    // Draw the ground track lines
    drawGroundTrackLines(groundTrack, trackColor, EARTH_RADIUS);
}

void GroundTrackRenderer::drawSubsatellitePoint(
    const Satellite& sat,
    Color pointColor
) {
    if (!sat.isVisible()) return;
    
    GeoCoordinate subsatPoint = GroundTrack::getSubsatellitePoint(
        sat.getCurrentState()
    );
    
    Vector3 surfacePos = latLonToSurfacePosition(
        subsatPoint.latitude,
        subsatPoint.longitude,
        EARTH_RADIUS
    );
    
    // Scale to render coordinates
    surfacePos.x *= SCALE;
    surfacePos.y *= SCALE;
    surfacePos.z *= SCALE;
    
    // Draw a small sphere at the subsatellite point
    DrawSphere(surfacePos, 0.15f, pointColor);
    
    // Draw a small vertical line to make it more visible
    Vector3 abovePoint = {
        surfacePos.x * 1.02f,
        surfacePos.y * 1.02f,
        surfacePos.z * 1.02f
    };
    DrawLine3D(surfacePos, abovePoint, pointColor);
}

void GroundTrackRenderer::drawCoverageCircle(
    const Satellite& sat,
    Color circleColor,
    double minElevationAngle
) {
    if (!sat.isVisible()) return;
    
    GeoCoordinate subsatPoint = GroundTrack::getSubsatellitePoint(
        sat.getCurrentState()
    );
    
    double coverageRadius = GroundTrack::calculateCoverageRadius(
        subsatPoint.altitude,
        minElevationAngle
    );
    
    drawSurfaceCircle(
        subsatPoint.latitude,
        subsatPoint.longitude,
        coverageRadius,
        circleColor,
        48  // More segments for smooth circle
    );
}

void GroundTrackRenderer::drawGroundTrackLines(
    const std::vector<GeoCoordinate>& groundTrack,
    Color lineColor,
    double earthRadius
) {
    if (groundTrack.size() < 2) return;
    
    // Draw lines connecting ground track points
    for (size_t i = 1; i < groundTrack.size(); i++) {
        const GeoCoordinate& prev = groundTrack[i - 1];
        const GeoCoordinate& curr = groundTrack[i];
        
        // Skip if longitude jumps more than 180° (crosses date line)
        if (fabs(curr.longitude - prev.longitude) > 180.0) {
            continue;
        }
        
        Vector3 prevPos = latLonToSurfacePosition(prev.latitude, prev.longitude, earthRadius);
        Vector3 currPos = latLonToSurfacePosition(curr.latitude, curr.longitude, earthRadius);
        
        // Scale to render coordinates
        prevPos.x *= SCALE;
        prevPos.y *= SCALE;
        prevPos.z *= SCALE;
        currPos.x *= SCALE;
        currPos.y *= SCALE;
        currPos.z *= SCALE;
        
        // Offset slightly above surface to avoid z-fighting
        float offset = 1.005f;
        prevPos.x *= offset;
        prevPos.y *= offset;
        prevPos.z *= offset;
        currPos.x *= offset;
        currPos.y *= offset;
        currPos.z *= offset;
        
        DrawLine3D(prevPos, currPos, lineColor);
    }
}

void GroundTrackRenderer::drawAllGroundTracks(
    const std::vector<Satellite>& satellites,
    size_t activeSatIndex,
    bool showCoverage
) {
    for (size_t i = 0; i < satellites.size(); i++) {
        if (!satellites[i].isVisible()) continue;
        
        bool isActive = (i == activeSatIndex);
        Color trackColor = isActive ? 
            satellites[i].getStats().familyColor : 
            Fade(satellites[i].getStats().familyColor, 0.4f);
        
        // Draw ground track
        drawGroundTrack(satellites[i], trackColor, isActive);
        
        // Draw subsatellite point
        drawSubsatellitePoint(satellites[i], trackColor);
        
        // Draw coverage circle (only for active satellite if enabled)
        if (showCoverage && isActive) {
            drawCoverageCircle(satellites[i], Fade(trackColor, 0.3f));
        }
    }
}

Vector3 GroundTrackRenderer::latLonToSurfacePosition(
    double latitude,
    double longitude,
    double earthRadius
) {
    double latRad = latitude * M_PI / 180.0;
    double lonRad = longitude * M_PI / 180.0;
    
    // Convert to Cartesian coordinates
    // Note: This is in ECI frame (no Earth rotation applied for rendering)
    float x = earthRadius * cos(latRad) * cos(lonRad);
    float y = earthRadius * cos(latRad) * sin(lonRad);
    float z = earthRadius * sin(latRad);
    
    return Vector3{x, y, z};
}

void GroundTrackRenderer::drawSurfaceCircle(
    double centerLat,
    double centerLon,
    double radiusKm,
    Color color,
    int segments
) {
    double earthRadius = EARTH_RADIUS;
    
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
        float offset = 1.006f;
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
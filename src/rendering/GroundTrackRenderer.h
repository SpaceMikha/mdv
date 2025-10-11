#ifndef GROUND_TRACK_RENDERER_H
#define GROUND_TRACK_RENDERER_H

#include "GroundTrack.h"
#include "Satellite.h"
#include "raylib.h"
#include <vector>

// Ground track rendering system
class GroundTrackRenderer {
public:
    // Draw ground track for a satellite
    static void drawGroundTrack(
        const Satellite& sat,
        Color trackColor,
        bool isActive = false
    );
    
    // Draw subsatellite point (current position on ground)
    static void drawSubsatellitePoint(
        const Satellite& sat,
        Color pointColor
    );
    
    // Draw coverage circle (area satellite can see)
    static void drawCoverageCircle(
        const Satellite& sat,
        Color circleColor,
        double minElevationAngle = 5.0
    );
    
    // Draw entire ground track as lines on Earth surface
    static void drawGroundTrackLines(
        const std::vector<GeoCoordinate>& groundTrack,
        Color lineColor,
        double earthRadius = 6378.137
    );
    
    // Draw ground track for all visible satellites
    static void drawAllGroundTracks(
        const std::vector<Satellite>& satellites,
        size_t activeSatIndex,
        bool showCoverage = true
    );
    
private:
    // Convert lat/lon to 3D position on Earth surface
    static Vector3 latLonToSurfacePosition(
        double latitude,
        double longitude,
        double earthRadius = 6378.137
    );
    
    // Draw a circle on Earth's surface at given lat/lon
    static void drawSurfaceCircle(
        double centerLat,
        double centerLon,
        double radiusKm,
        Color color,
        int segments = 32
    );
};

#endif // GROUND_TRACK_RENDERER_H
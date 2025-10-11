#ifndef GROUND_STATION_RENDERER_H
#define GROUND_STATION_RENDERER_H

#include "GroundStation.h"
#include "Satellite.h"
#include "raylib.h"
#include <vector>

// Ground station rendering system
class GroundStationRenderer {
public:
    // Draw a ground station marker on Earth
    static void drawGroundStation(
        const GroundStation& station,
        double earthRadius = 6378.137
    );
    
    // Draw communication cone (area satellite can be seen)
    static void drawCommunicationCone(
        const GroundStation& station,
        double earthRadius = 6378.137,
        int segments = 32
    );
    
    // Draw line-of-sight line from station to satellite
    static void drawLineOfSight(
        const GroundStation& station,
        const Satellite& satellite,
        double earthRadius = 6378.137,
        double currentTime = 0.0
    );
    
    // Draw all ground stations
    static void drawAllGroundStations(
        const std::vector<GroundStation>& stations,
        bool showCones = true,
        double earthRadius = 6378.137
    );
    
    // Draw access visualization for a satellite-station pair
    static void drawAccessVisualization(
        const GroundStation& station,
        const Satellite& satellite,
        const AccessStatistics& stats,
        double earthRadius = 6378.137,
        double currentTime = 0.0
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
        int segments = 32,
        double earthRadius = 6378.137
    );
};

#endif // GROUND_STATION_RENDERER_H
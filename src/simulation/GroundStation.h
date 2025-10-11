#ifndef GROUND_STATION_H
#define GROUND_STATION_H

#include "Vector3D.h"
#include "StateVector.h"
#include "GroundTrack.h"
#include "raylib.h"
#include <string>
#include <vector>

// Ground station structure
struct GroundStation {
    std::string name;
    std::string code;        // e.g., "JPL", "MAD", "USD"
    GeoCoordinate location;  // Latitude, longitude, altitude
    double minElevation;     // Minimum elevation angle (degrees)
    Color color;
    bool visible;
    
    GroundStation()
        : name(""), code(""), location(), minElevation(5.0), 
          color(WHITE), visible(true) {}
    
    GroundStation(const std::string& n, const std::string& c, 
                  double lat, double lon, double alt = 0.0,
                  double minElev = 5.0, Color col = WHITE)
        : name(n), code(c), 
          location(lat, lon, alt),
          minElevation(minElev), color(col), visible(true) {}
};

// Access window - when satellite can communicate with station
struct AccessWindow {
    double startTime;      // seconds
    double endTime;        // seconds
    double duration;       // seconds
    double maxElevation;   // degrees
    size_t startFrame;
    size_t endFrame;
    
    AccessWindow(double start, double end, double maxElev, 
                 size_t startF, size_t endF)
        : startTime(start), endTime(end), 
          duration(end - start), maxElevation(maxElev),
          startFrame(startF), endFrame(endF) {}
};

// Access statistics for a satellite-station pair
struct AccessStatistics {
    std::vector<AccessWindow> windows;
    double totalAccessTime;    // seconds per orbit
    int passesPerOrbit;
    double averagePassDuration;
    double longestPass;
    double shortestPass;
    
    AccessStatistics()
        : totalAccessTime(0.0), passesPerOrbit(0),
          averagePassDuration(0.0), longestPass(0.0), 
          shortestPass(1e10) {}
    
    void calculate() {
        if (windows.empty()) {
            passesPerOrbit = 0;
            totalAccessTime = 0.0;
            averagePassDuration = 0.0;
            longestPass = 0.0;
            shortestPass = 0.0;
            return;
        }
        
        passesPerOrbit = windows.size();
        totalAccessTime = 0.0;
        longestPass = 0.0;
        shortestPass = 1e10;
        
        for (const auto& window : windows) {
            totalAccessTime += window.duration;
            if (window.duration > longestPass) longestPass = window.duration;
            if (window.duration < shortestPass) shortestPass = window.duration;
        }
        
        averagePassDuration = totalAccessTime / passesPerOrbit;
    }
};

// Ground station access calculator
class GroundStationAccess {
public:
    // Check if satellite is visible from ground station
    static bool isVisible(
        const Vector3D& satPosition,
        const GroundStation& station,
        double earthRadius,
        double timeSeconds = 0.0
    );
    
    // Calculate elevation angle from station to satellite
    static double calculateElevation(
        const Vector3D& satPosition,
        const GroundStation& station,
        double earthRadius,
        double timeSeconds = 0.0
    );
    
    // Calculate azimuth angle from station to satellite
    static double calculateAzimuth(
        const Vector3D& satPosition,
        const GroundStation& station,
        double earthRadius,
        double timeSeconds = 0.0
    );
    
    // Calculate all access windows for one orbit
    static AccessStatistics calculateAccessWindows(
        const std::vector<StateVector>& orbit,
        const GroundStation& station,
        double earthRadius
    );
    
    // Calculate range (distance) from station to satellite
    static double calculateRange(
        const Vector3D& satPosition,
        const GroundStation& station,
        double earthRadius,
        double timeSeconds = 0.0
    );
};

// Preset ground stations
class GroundStationPresets {
public:
    enum StationType {
        NASA_JPL,      // Jet Propulsion Laboratory, California
        ESA_MADRID,    // European Space Agency, Spain
        JAXA_USUDA,    // Japan Aerospace Exploration Agency
        NASA_WALLOPS,  // Wallops Flight Facility, Virginia
        ESA_KOUROU,    // Guiana Space Centre, French Guiana
        STATION_COUNT
    };
    
    static GroundStation createStation(StationType type);
    static std::vector<GroundStation> getAllStations();
    static std::string getStationName(StationType type);
};

#endif // GROUND_STATION_H
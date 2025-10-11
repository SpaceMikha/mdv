#ifndef GROUND_TRACK_H
#define GROUND_TRACK_H

#include "Vector3D.h"
#include "StateVector.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Geographic coordinates
struct GeoCoordinate {
    double latitude;   // degrees (-90 to +90)
    double longitude;  // degrees (-180 to +180)
    double altitude;   // km above surface
    
    GeoCoordinate() : latitude(0.0), longitude(0.0), altitude(0.0) {}
    GeoCoordinate(double lat, double lon, double alt = 0.0) 
        : latitude(lat), longitude(lon), altitude(alt) {}
};

// Ground track utilities
class GroundTrack {
public:
    // Convert ECI (Earth-Centered Inertial) position to latitude/longitude
    // Takes into account Earth's rotation
    static GeoCoordinate ECIToLatLon(const Vector3D& eciPosition, double timeSeconds);
    
    // Convert lat/lon/alt to ECI position (for ground stations, etc.)
    static Vector3D LatLonToECI(const GeoCoordinate& coord, double timeSeconds);
    
    // Calculate subsatellite point (directly below satellite)
    static GeoCoordinate getSubsatellitePoint(const StateVector& state);
    
    // Calculate ground track for entire orbit
    static std::vector<GeoCoordinate> calculateGroundTrack(
        const std::vector<StateVector>& orbit,
        int samplesPerOrbit = 360
    );
    
    // Calculate coverage radius (satellite's horizon distance)
    // altitude: satellite altitude in km
    // minElevationAngle: minimum elevation angle in degrees (typically 5-10°)
    static double calculateCoverageRadius(double altitude, double minElevationAngle = 5.0);
    
    // Calculate if a ground point is visible from satellite
    static bool isGroundPointVisible(
        const Vector3D& satPosition,
        const GeoCoordinate& groundPoint,
        double earthRadius,
        double minElevationAngle = 5.0
    );
    
    // Get ground track segment between two times (handles Earth rotation)
    static std::vector<GeoCoordinate> getGroundTrackSegment(
        const std::vector<StateVector>& orbit,
        size_t startFrame,
        size_t endFrame
    );
    
private:
    // Earth rotation rate (radians per second)
    static constexpr double EARTH_ROTATION_RATE = 7.2921159e-5; // rad/s
    
    // Normalize longitude to [-180, +180]
    static double normalizeLongitude(double lon);
};

#endif // GROUND_TRACK_H
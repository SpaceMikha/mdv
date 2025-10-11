#include "GroundTrack.h"
#include <cmath>

GeoCoordinate GroundTrack::ECIToLatLon(const Vector3D& eciPosition, double timeSeconds) {
    GeoCoordinate coord;
    
    double x = eciPosition.x;
    double y = eciPosition.y;
    double z = eciPosition.z;
    
    double r = eciPosition.magnitude();
    
    // Latitude (geocentric)
    coord.latitude = asin(z / r) * 180.0 / M_PI;
    
    // Longitude (account for Earth's rotation)
    double lon = atan2(y, x) * 180.0 / M_PI;
    
    // Subtract Earth rotation since epoch
    double earthRotationDeg = (EARTH_ROTATION_RATE * timeSeconds) * 180.0 / M_PI;
    coord.longitude = normalizeLongitude(lon - earthRotationDeg);
    
    // Altitude
    coord.altitude = r - 6378.137; // Subtract Earth radius
    
    return coord;
}

Vector3D GroundTrack::LatLonToECI(const GeoCoordinate& coord, double timeSeconds) {
    double earthRadius = 6378.137; // km
    double r = earthRadius + coord.altitude;
    
    double latRad = coord.latitude * M_PI / 180.0;
    double lonRad = coord.longitude * M_PI / 180.0;
    
    // Add Earth rotation
    double earthRotationRad = EARTH_ROTATION_RATE * timeSeconds;
    lonRad += earthRotationRad;
    
    // Convert spherical to Cartesian
    double x = r * cos(latRad) * cos(lonRad);
    double y = r * cos(latRad) * sin(lonRad);
    double z = r * sin(latRad);
    
    return Vector3D(x, y, z);
}

GeoCoordinate GroundTrack::getSubsatellitePoint(const StateVector& state) {
    return ECIToLatLon(state.position, state.time);
}

std::vector<GeoCoordinate> GroundTrack::calculateGroundTrack(
    const std::vector<StateVector>& orbit,
    int samplesPerOrbit
) {
    std::vector<GeoCoordinate> groundTrack;
    
    if (orbit.empty()) return groundTrack;
    
    // Sample the orbit at regular intervals
    size_t step = orbit.size() / samplesPerOrbit;
    if (step < 1) step = 1;
    
    for (size_t i = 0; i < orbit.size(); i += step) {
        GeoCoordinate coord = getSubsatellitePoint(orbit[i]);
        groundTrack.push_back(coord);
    }
    
    return groundTrack;
}

double GroundTrack::calculateCoverageRadius(double altitude, double minElevationAngle) {
    double earthRadius = 6378.137; // km
    
    // Convert elevation angle to radians
    double elevRad = minElevationAngle * M_PI / 180.0;
    
    // Calculate Earth central angle to horizon
    double rho = acos(earthRadius / (earthRadius + altitude) * cos(elevRad)) - elevRad;
    
    // Arc length on Earth's surface
    double coverageRadius = earthRadius * rho;
    
    return coverageRadius;
}

bool GroundTrack::isGroundPointVisible(
    const Vector3D& satPosition,
    const GeoCoordinate& groundPoint,
    double earthRadius,
    double minElevationAngle
) {
    // Convert ground point to ECI (assuming current time = 0 for simplicity)
    Vector3D groundECI = LatLonToECI(groundPoint, 0.0);
    
    // Vector from ground point to satellite
    Vector3D toSat = satPosition - groundECI;
    
    // Local vertical at ground point (normalized position vector)
    Vector3D localVertical = groundECI.normalized();
    
    // Calculate elevation angle
    double elevationRad = asin(toSat.normalized().dot(localVertical));
    double elevationDeg = elevationRad * 180.0 / M_PI;
    
    return (elevationDeg >= minElevationAngle);
}

std::vector<GeoCoordinate> GroundTrack::getGroundTrackSegment(
    const std::vector<StateVector>& orbit,
    size_t startFrame,
    size_t endFrame
) {
    std::vector<GeoCoordinate> segment;
    
    if (orbit.empty() || startFrame >= orbit.size()) return segment;
    if (endFrame >= orbit.size()) endFrame = orbit.size() - 1;
    
    for (size_t i = startFrame; i <= endFrame; i++) {
        segment.push_back(getSubsatellitePoint(orbit[i]));
    }
    
    return segment;
}

double GroundTrack::normalizeLongitude(double lon) {
    while (lon > 180.0) lon -= 360.0;
    while (lon < -180.0) lon += 360.0;
    return lon;
}
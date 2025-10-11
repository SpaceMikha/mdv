#include "GroundStation.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool GroundStationAccess::isVisible(
    const Vector3D& satPosition,
    const GroundStation& station,
    double earthRadius,
    double timeSeconds
) {
    double elevation = calculateElevation(satPosition, station, earthRadius, timeSeconds);
    return elevation >= station.minElevation;
}

double GroundStationAccess::calculateElevation(
    const Vector3D& satPosition,
    const GroundStation& station,
    double earthRadius,
    double timeSeconds
) {
    // Convert station to ECI coordinates
    Vector3D stationECI = GroundTrack::LatLonToECI(station.location, timeSeconds);
    
    // Vector from station to satellite
    Vector3D toSat = satPosition - stationECI;
    
    // Local vertical at station (normalized position vector)
    Vector3D localVertical = stationECI.normalized();
    
    // Calculate elevation angle
    double elevationRad = asin(toSat.normalized().dot(localVertical));
    return elevationRad * 180.0 / M_PI;
}

double GroundStationAccess::calculateAzimuth(
    const Vector3D& satPosition,
    const GroundStation& station,
    double earthRadius,
    double timeSeconds
) {
    // Convert station to ECI coordinates
    Vector3D stationECI = GroundTrack::LatLonToECI(station.location, timeSeconds);
    
    // Vector from station to satellite
    Vector3D toSat = satPosition - stationECI;
    
    // Local coordinate frame at station
    Vector3D localVertical = stationECI.normalized();
    Vector3D eastDirection = Vector3D(0, 0, 1).cross(localVertical).normalized();
    Vector3D northDirection = localVertical.cross(eastDirection).normalized();
    
    // Project toSat onto local horizontal plane
    Vector3D toSatHorizontal = toSat - localVertical * toSat.dot(localVertical);
    toSatHorizontal = toSatHorizontal.normalized();
    
    // Calculate azimuth (angle from north, clockwise)
    double azimuthRad = atan2(toSatHorizontal.dot(eastDirection), 
                              toSatHorizontal.dot(northDirection));
    double azimuthDeg = azimuthRad * 180.0 / M_PI;
    
    // Convert to 0-360 range
    if (azimuthDeg < 0.0) azimuthDeg += 360.0;
    
    return azimuthDeg;
}

AccessStatistics GroundStationAccess::calculateAccessWindows(
    const std::vector<StateVector>& orbit,
    const GroundStation& station,
    double earthRadius
) {
    AccessStatistics stats;
    
    if (orbit.empty()) return stats;
    
    bool inAccess = false;
    double accessStartTime = 0.0;
    size_t accessStartFrame = 0;
    double maxElevation = 0.0;
    
    for (size_t i = 0; i < orbit.size(); ++i) {
        bool visible = isVisible(orbit[i].position, station, earthRadius, orbit[i].time);
        double elevation = calculateElevation(orbit[i].position, station, earthRadius, orbit[i].time);
        
        if (visible && !inAccess) {
            // Access window starts
            inAccess = true;
            accessStartTime = orbit[i].time;
            accessStartFrame = i;
            maxElevation = elevation;
        }
        else if (visible && inAccess) {
            // Continue tracking max elevation
            if (elevation > maxElevation) {
                maxElevation = elevation;
            }
        }
        else if (!visible && inAccess) {
            // Access window ends
            inAccess = false;
            stats.windows.emplace_back(
                accessStartTime,
                orbit[i-1].time,
                maxElevation,
                accessStartFrame,
                i-1
            );
        }
    }
    
    // Handle case where access continues to end of orbit
    if (inAccess) {
        stats.windows.emplace_back(
            accessStartTime,
            orbit.back().time,
            maxElevation,
            accessStartFrame,
            orbit.size() - 1
        );
    }
    
    stats.calculate();
    return stats;
}

double GroundStationAccess::calculateRange(
    const Vector3D& satPosition,
    const GroundStation& station,
    double earthRadius,
    double timeSeconds
) {
    Vector3D stationECI = GroundTrack::LatLonToECI(station.location, timeSeconds);
    return satPosition.distance(stationECI);
}

// Preset ground stations
GroundStation GroundStationPresets::createStation(StationType type) {
    switch (type) {
        case NASA_JPL:
            return GroundStation(
                "NASA JPL",
                "JPL",
                35.4,    // Goldstone, CA (Deep Space Network)
                -116.9,
                0.0,
                5.0,
                Color{100, 200, 255, 255}  // Light blue
            );
        
        case ESA_MADRID:
            return GroundStation(
                "ESA Madrid",
                "MAD",
                40.4,    // Cebreros, Spain
                -4.4,
                0.0,
                5.0,
                Color{255, 200, 100, 255}  // Light orange
            );
        
        case JAXA_USUDA:
            return GroundStation(
                "JAXA Usuda",
                "USD",
                36.1,    // Usuda, Japan
                138.4,
                0.0,
                5.0,
                Color{255, 100, 100, 255}  // Light red
            );
        
        case NASA_WALLOPS:
            return GroundStation(
                "NASA Wallops",
                "WLP",
                37.9,    // Wallops Island, Virginia
                -75.5,
                0.0,
                5.0,
                Color{100, 255, 100, 255}  // Light green
            );
        
        case ESA_KOUROU:
            return GroundStation(
                "ESA Kourou",
                "KOU",
                5.2,     // French Guiana
                -52.8,
                0.0,
                5.0,
                Color{200, 100, 255, 255}  // Light purple
            );
        
        default:
            return GroundStation();
    }
}

std::vector<GroundStation> GroundStationPresets::getAllStations() {
    std::vector<GroundStation> stations;
    for (int i = 0; i < STATION_COUNT; ++i) {
        stations.push_back(createStation(static_cast<StationType>(i)));
    }
    return stations;
}

std::string GroundStationPresets::getStationName(StationType type) {
    switch (type) {
        case NASA_JPL: return "NASA JPL";
        case ESA_MADRID: return "ESA Madrid";
        case JAXA_USUDA: return "JAXA Usuda";
        case NASA_WALLOPS: return "NASA Wallops";
        case ESA_KOUROU: return "ESA Kourou";
        default: return "Unknown";
    }
}
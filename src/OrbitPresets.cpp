#include "OrbitPresets.h"
#include <cmath>
#include "raylib.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const double EARTH_RADIUS = 6378.137; // km

// Helper function to create state vector from orbital parameters
StateVector createStateFromOrbitalParams(
    double altitude,           // km above Earth's surface
    double inclination,        // degrees
    double eccentricity,       // dimensionless
    double argumentOfPeriapsis, // degrees
    double mu
) {
    // Convert to radians
    double inc = inclination * M_PI / 180.0;
    double omega = argumentOfPeriapsis * M_PI / 180.0;
    
    // Semi-major axis
    double a = EARTH_RADIUS + altitude;
    
    // For eccentric orbits, adjust periapsis
    if (eccentricity > 0.0) {
        // altitude is periapsis altitude
        double rp = EARTH_RADIUS + altitude;
        a = rp / (1.0 - eccentricity);
    }
    
    // Initial position at periapsis in orbital plane
    double r = a * (1.0 - eccentricity);
    
    // Velocity at periapsis
    double v = std::sqrt(mu * (2.0/r - 1.0/a));
    
    // Position in orbital plane (at periapsis)
    Vector3D pos_orbit(r, 0.0, 0.0);
    Vector3D vel_orbit(0.0, v, 0.0);
    
    // Rotate by argument of periapsis (in orbital plane)
    double cos_omega = std::cos(omega);
    double sin_omega = std::sin(omega);
    Vector3D pos_rotated(
        pos_orbit.x * cos_omega - pos_orbit.y * sin_omega,
        pos_orbit.x * sin_omega + pos_orbit.y * cos_omega,
        0.0
    );
    Vector3D vel_rotated(
        vel_orbit.x * cos_omega - vel_orbit.y * sin_omega,
        vel_orbit.x * sin_omega + vel_orbit.y * cos_omega,
        0.0
    );
    
    // Rotate by inclination (around x-axis)
    double cos_inc = std::cos(inc);
    double sin_inc = std::sin(inc);
    Vector3D pos_final(
        pos_rotated.x,
        pos_rotated.y * cos_inc - pos_rotated.z * sin_inc,
        pos_rotated.y * sin_inc + pos_rotated.z * cos_inc
    );
    Vector3D vel_final(
        vel_rotated.x,
        vel_rotated.y * cos_inc - vel_rotated.z * sin_inc,
        vel_rotated.y * sin_inc + vel_rotated.z * cos_inc
    );
    
    return StateVector(pos_final, vel_final, 0.0);
}

OrbitPreset OrbitPresets::createPreset(OrbitType type, double mu) {
    switch (type) {
        case ORBIT_ISS: {
            // ISS: ~400 km altitude, 51.6° inclination
            StateVector state = createStateFromOrbitalParams(400.0, 51.6, 0.0, 0.0, mu);
            double a = EARTH_RADIUS + 400.0;
            double period = 2.0 * M_PI * std::sqrt(a*a*a / mu);
            return OrbitPreset(ORBIT_ISS, "ISS", 
                             "Low Earth Orbit, 400 km altitude, 51.6° inclination",
                             state, period, YELLOW);
        }
        
        case ORBIT_GEO: {
            // GEO: 35,786 km altitude, 0° inclination
            StateVector state = createStateFromOrbitalParams(35786.0, 0.0, 0.0, 0.0, mu);
            double period = 86164.0; // Sidereal day in seconds
            return OrbitPreset(ORBIT_GEO, "GEO",
                             "Geostationary Orbit, 35,786 km altitude, 0° inclination",
                             state, period, ORANGE);
        }
        
        case ORBIT_MOLNIYA: {
            // Molniya: Highly eccentric, 63.4° inclination
            // Periapsis ~500 km, Apoapsis ~39,900 km
            StateVector state = createStateFromOrbitalParams(500.0, 63.4, 0.737, 270.0, mu);
            double a = (EARTH_RADIUS + 500.0 + EARTH_RADIUS + 39900.0) / 2.0;
            double period = 2.0 * M_PI * std::sqrt(a*a*a / mu);
            return OrbitPreset(ORBIT_MOLNIYA, "Molniya",
                             "Highly elliptical, 500-39,900 km, 63.4° inclination",
                             state, period, RED);
        }
        
        case ORBIT_GPS: {
            // GPS: ~20,200 km altitude, 55° inclination
            StateVector state = createStateFromOrbitalParams(20200.0, 55.0, 0.0, 0.0, mu);
            double a = EARTH_RADIUS + 20200.0;
            double period = 2.0 * M_PI * std::sqrt(a*a*a / mu);
            return OrbitPreset(ORBIT_GPS, "GPS",
                             "Medium Earth Orbit, 20,200 km altitude, 55° inclination",
                             state, period, GREEN);
        }
        
        case ORBIT_SUNSYNC: {
            // Sun-Synchronous: ~600 km altitude, 98° inclination
            StateVector state = createStateFromOrbitalParams(600.0, 98.0, 0.0, 0.0, mu);
            double a = EARTH_RADIUS + 600.0;
            double period = 2.0 * M_PI * std::sqrt(a*a*a / mu);
            return OrbitPreset(ORBIT_SUNSYNC, "Sun-Sync",
                             "Sun-Synchronous, 600 km altitude, 98° inclination",
                             state, period, SKYBLUE);
        }
        
        case ORBIT_POLAR: {
            // Polar: ~600 km altitude, 90° inclination
            StateVector state = createStateFromOrbitalParams(600.0, 90.0, 0.0, 0.0, mu);
            double a = EARTH_RADIUS + 600.0;
            double period = 2.0 * M_PI * std::sqrt(a*a*a / mu);
            return OrbitPreset(ORBIT_POLAR, "Polar",
                             "Polar Orbit, 600 km altitude, 90° inclination",
                             state, period, PURPLE);
        }
        
        case ORBIT_TUNDRA: {
            // Tundra: Highly eccentric, 63.4° inclination, ~24h period
            // Periapsis ~20,000 km, Apoapsis ~46,000 km
            StateVector state = createStateFromOrbitalParams(20000.0, 63.4, 0.27, 270.0, mu);
            double a = (EARTH_RADIUS + 20000.0 + EARTH_RADIUS + 46000.0) / 2.0;
            double period = 2.0 * M_PI * std::sqrt(a*a*a / mu);
            return OrbitPreset(ORBIT_TUNDRA, "Tundra",
                             "Tundra Orbit, 20,000-46,000 km, 63.4° incl, 24h period",
                             state, period, PINK);
        }
        
        case ORBIT_GTO: {
            // GTO: Geostationary Transfer Orbit
            // Periapsis ~200 km, Apoapsis ~35,786 km (GEO altitude)
            StateVector state = createStateFromOrbitalParams(200.0, 7.0, 0.73, 180.0, mu);
            double a = (EARTH_RADIUS + 200.0 + EARTH_RADIUS + 35786.0) / 2.0;
            double period = 2.0 * M_PI * std::sqrt(a*a*a / mu);
            return OrbitPreset(ORBIT_GTO, "GTO",
                             "Geostationary Transfer, 200-35,786 km, 7° inclination",
                             state, period, LIME);
        }
        
        case ORBIT_HUBBLE: {
            // Hubble Space Telescope: ~540 km altitude, 28.5° inclination
            StateVector state = createStateFromOrbitalParams(540.0, 28.5, 0.0, 0.0, mu);
            double a = EARTH_RADIUS + 540.0;
            double period = 2.0 * M_PI * std::sqrt(a*a*a / mu);
            return OrbitPreset(ORBIT_HUBBLE, "Hubble",
                             "Hubble Space Telescope, 540 km altitude, 28.5° incl",
                             state, period, GOLD);
        }
        
        case ORBIT_STARLINK: {
            // Starlink: ~550 km altitude, 53° inclination
            StateVector state = createStateFromOrbitalParams(550.0, 53.0, 0.0, 0.0, mu);
            double a = EARTH_RADIUS + 550.0;
            double period = 2.0 * M_PI * std::sqrt(a*a*a / mu);
            return OrbitPreset(ORBIT_STARLINK, "Starlink",
                             "Starlink Constellation, 550 km altitude, 53° incl",
                             state, period, MAROON);
        }
        
        default:
            return createPreset(ORBIT_ISS, mu);
    }
}

std::vector<OrbitPreset> OrbitPresets::getAllPresets(double mu) {
    std::vector<OrbitPreset> presets;
    for (int i = 0; i < ORBIT_COUNT; i++) {
        presets.push_back(createPreset(static_cast<OrbitType>(i), mu));
    }
    return presets;
}

std::string OrbitPresets::getPresetName(OrbitType type) {
    switch (type) {
        case ORBIT_ISS: return "ISS";
        case ORBIT_GEO: return "GEO";
        case ORBIT_MOLNIYA: return "Molniya";
        case ORBIT_GPS: return "GPS";
        case ORBIT_SUNSYNC: return "Sun-Sync";
        case ORBIT_POLAR: return "Polar";
        case ORBIT_TUNDRA: return "Tundra";
        case ORBIT_GTO: return "GTO";
        case ORBIT_HUBBLE: return "Hubble";
        case ORBIT_STARLINK: return "Starlink";
        default: return "Unknown";
    }
}
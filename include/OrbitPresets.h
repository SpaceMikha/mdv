#ifndef ORBIT_PRESETS_H
#define ORBIT_PRESETS_H

#include "StateVector.h"
#include "raylib.h"
#include <string>
#include <vector>

/**
 * Common orbit types and their initial state vectors
 */
enum OrbitType {
    ORBIT_ISS,           // International Space Station (~400 km, 51.6° inclination)
    ORBIT_GEO,           // Geostationary Orbit (35,786 km altitude, 0° inclination)
    ORBIT_MOLNIYA,       // Molniya Orbit (highly eccentric, 63.4° inclination)
    ORBIT_GPS,           // GPS Constellation (20,200 km, 55° inclination)
    ORBIT_SUNSYNC,       // Sun-Synchronous (600 km altitude, 98° inclination)
    ORBIT_POLAR,         // Polar Orbit (600 km altitude, 90° inclination)
    ORBIT_TUNDRA,        // Tundra Orbit (highly eccentric, 63.4° inclination, 24h period)
    ORBIT_GTO,           // Geostationary Transfer Orbit (200 km x 35,786 km)
    ORBIT_HUBBLE,        // Hubble Space Telescope (540 km, 28.5° inclination)
    ORBIT_STARLINK,      // Starlink Constellation (550 km, 53° inclination)
    ORBIT_COUNT
};

struct OrbitPreset {
    OrbitType type;
    std::string name;
    std::string description;
    StateVector initialState;
    double period;          // Orbital period in seconds
    Color color;            // Color for visualization
    
    OrbitPreset(OrbitType t, const std::string& n, const std::string& desc, 
                const StateVector& state, double per, Color col)
        : type(t), name(n), description(desc), initialState(state), 
          period(per), color(col) {}
};

class OrbitPresets {
public:
    // Create a preset orbit
    static OrbitPreset createPreset(OrbitType type, double mu);
    
    // Get all available presets
    static std::vector<OrbitPreset> getAllPresets(double mu);
    
    // Get preset name
    static std::string getPresetName(OrbitType type);
};

#endif // ORBIT_PRESETS_H
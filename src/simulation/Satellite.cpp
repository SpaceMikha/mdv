#include "Satellite.h"
#include "Constants.h"

Satellite::Satellite(const OrbitPreset& p, const std::vector<StateVector>& o)
    : orbit(o), currentFrame(0), preset(p), visible(true) {
    calculateStatistics(EARTH_RADIUS);
}

void Satellite::advanceFrames(size_t frames) {
    if (visible && !orbit.empty()) {
        currentFrame = (currentFrame + frames) % orbit.size();
    }
}

void Satellite::calculateStatistics(double earthRadius) {
    if (orbit.empty()) return;
    
    double minR = 1e10, maxR = 0;
    size_t periIdx = 0, apoIdx = 0;
    
    // Find periapsis and apoapsis
    for (size_t i = 0; i < orbit.size(); i++) {
        double r = orbit[i].position.magnitude();
        if (r < minR) {
            minR = r;
            periIdx = i;
        }
        if (r > maxR) {
            maxR = r;
            apoIdx = i;
        }
    }
    
    // Calculate altitudes
    stats.periapsisAlt = minR - earthRadius;
    stats.apoapsisAlt = maxR - earthRadius;
    stats.periapsisVel = orbit[periIdx].velocity.magnitude();
    stats.apoapsisVel = orbit[apoIdx].velocity.magnitude();
    stats.meanAltitude = (stats.periapsisAlt + stats.apoapsisAlt) / 2.0;
    
    // Classify orbit family
    if (stats.meanAltitude < LEO_MAX_ALTITUDE) {
        stats.orbitFamily = "LEO";
        stats.familyColor = Color{100, 200, 255, 255};
    } else if (stats.meanAltitude < MEO_MAX_ALTITUDE) {
        stats.orbitFamily = "MEO";
        stats.familyColor = Color{100, 255, 100, 255};
    } else if (stats.apoapsisAlt > MEO_MAX_ALTITUDE && stats.periapsisAlt < MEO_MAX_ALTITUDE) {
        stats.orbitFamily = "HEO";
        stats.familyColor = Color{255, 150, 100, 255};
    } else {
        stats.orbitFamily = "GEO";
        stats.familyColor = Color{255, 100, 255, 255};
    }
}
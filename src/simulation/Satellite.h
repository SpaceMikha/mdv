#ifndef SATELLITE_H
#define SATELLITE_H

#include <vector>
#include <string>
#include "StateVector.h"
#include "OrbitPresets.h"
#include "raylib.h"

// Orbit statistics
struct OrbitStatistics {
    double periapsisAlt;
    double apoapsisAlt;
    double periapsisVel;
    double apoapsisVel;
    double meanAltitude;
    std::string orbitFamily;
    Color familyColor;
};

// Satellite class - encapsulates all satellite data and behavior
class Satellite {
public:
    // Constructor
    Satellite(const OrbitPreset& preset, const std::vector<StateVector>& orbit);
    
    // Getters
    const std::vector<StateVector>& getOrbit() const { return orbit; }
    size_t getCurrentFrame() const { return currentFrame; }
    const StateVector& getCurrentState() const { return orbit[currentFrame]; }
    const OrbitPreset& getPreset() const { return preset; }
    const OrbitStatistics& getStats() const { return stats; }
    bool isVisible() const { return visible; }
    
    // Setters
    void setVisible(bool vis) { visible = vis; }
    void setCurrentFrame(size_t frame) { 
        if (!orbit.empty()) {
            currentFrame = frame % orbit.size(); 
        }
    }
    
    // Update
    void advanceFrames(size_t frames);
    
    // Statistics
    void calculateStatistics(double earthRadius);
    
private:
    std::vector<StateVector> orbit;
    size_t currentFrame;
    OrbitPreset preset;
    bool visible;
    OrbitStatistics stats;
};

#endif // SATELLITE_H
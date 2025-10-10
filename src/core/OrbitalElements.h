#ifndef ORBITAL_ELEMENTS_H
#define ORBITAL_ELEMENTS_H

#include "StateVector.h"
#include <string>

/**
 * Classical Orbital Elements (Keplerian Elements)
 * Computed from Cartesian state vectors
 */
class OrbitalElements {
public:
    double semiMajorAxis;        // a (km)
    double eccentricity;         // e (dimensionless)
    double inclination;          // i (radians)
    double rightAscension;       // Ω - RAAN (radians)
    double argumentOfPeriapsis;  // ω (radians)
    double trueAnomaly;          // ν (radians)
    
    // Derived quantities
    double periapsis;            // rp (km)
    double apoapsis;             // ra (km)
    double period;               // T (seconds)
    
    // Constructors
    OrbitalElements();
    
    // Compute orbital elements from state vector
    static OrbitalElements fromStateVector(
        const StateVector& state,
        double mu
    );
    
    // Convert angles to degrees for display
    double inclinationDeg() const;
    double raanDeg() const;
    double argumentOfPeriapsisDeg() const;
    double trueAnomalyDeg() const;
    
    // Get orbit type as string
    std::string orbitType() const;
    
    // Print all elements
    void print() const;
};

#endif // ORBITAL_ELEMENTS_H
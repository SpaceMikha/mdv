#include "OrbitalElements.h"
#include <cmath>
#include <iostream>
#include <iomanip>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constructor
OrbitalElements::OrbitalElements()
    : semiMajorAxis(0.0), eccentricity(0.0), inclination(0.0),
      rightAscension(0.0), argumentOfPeriapsis(0.0), trueAnomaly(0.0),
      periapsis(0.0), apoapsis(0.0), period(0.0) {}

// Compute orbital elements from Cartesian state vector
OrbitalElements OrbitalElements::fromStateVector(
    const StateVector& state,
    double mu
) {
    OrbitalElements elements;
    
    Vector3D r = state.position;
    Vector3D v = state.velocity;
    
    double rMag = r.magnitude();
    double vMag = v.magnitude();
    
    // Specific angular momentum vector
    Vector3D h = r.cross(v);
    double hMag = h.magnitude();
    
    // Node vector (points to ascending node)
    Vector3D k(0.0, 0.0, 1.0);  // z-axis
    Vector3D n = k.cross(h);
    double nMag = n.magnitude();
    
    // Eccentricity vector
    Vector3D eVec = (v.cross(h) / mu) - r.normalized();
    elements.eccentricity = eVec.magnitude();
    
    // Specific orbital energy
    double energy = vMag * vMag / 2.0 - mu / rMag;
    
    // Semi-major axis
    elements.semiMajorAxis = -mu / (2.0 * energy);
    
    // Inclination
    elements.inclination = std::acos(h.z / hMag);
    
    // Right Ascension of Ascending Node (RAAN)
    if (nMag > 1e-10) {
        elements.rightAscension = std::acos(n.x / nMag);
        if (n.y < 0.0) {
            elements.rightAscension = 2.0 * M_PI - elements.rightAscension;
        }
    } else {
        elements.rightAscension = 0.0;  // Equatorial orbit
    }
    
    // Argument of periapsis
    if (nMag > 1e-10 && elements.eccentricity > 1e-10) {
        elements.argumentOfPeriapsis = std::acos(n.dot(eVec) / (nMag * elements.eccentricity));
        if (eVec.z < 0.0) {
            elements.argumentOfPeriapsis = 2.0 * M_PI - elements.argumentOfPeriapsis;
        }
    } else {
        elements.argumentOfPeriapsis = 0.0;
    }
    
    // True anomaly
    if (elements.eccentricity > 1e-10) {
        elements.trueAnomaly = std::acos(eVec.dot(r) / (elements.eccentricity * rMag));
        if (r.dot(v) < 0.0) {
            elements.trueAnomaly = 2.0 * M_PI - elements.trueAnomaly;
        }
    } else {
        // Circular orbit - use argument of latitude
        if (nMag > 1e-10) {
            elements.trueAnomaly = std::acos(n.dot(r) / (nMag * rMag));
            if (r.z < 0.0) {
                elements.trueAnomaly = 2.0 * M_PI - elements.trueAnomaly;
            }
        } else {
            elements.trueAnomaly = std::atan2(r.y, r.x);
            if (elements.trueAnomaly < 0.0) {
                elements.trueAnomaly += 2.0 * M_PI;
            }
        }
    }
    
    // Derived quantities
    elements.periapsis = elements.semiMajorAxis * (1.0 - elements.eccentricity);
    elements.apoapsis = elements.semiMajorAxis * (1.0 + elements.eccentricity);
    elements.period = 2.0 * M_PI * std::sqrt(
        elements.semiMajorAxis * elements.semiMajorAxis * elements.semiMajorAxis / mu
    );
    
    return elements;
}

// Convert to degrees
double OrbitalElements::inclinationDeg() const {
    return inclination * 180.0 / M_PI;
}

double OrbitalElements::raanDeg() const {
    return rightAscension * 180.0 / M_PI;
}

double OrbitalElements::argumentOfPeriapsisDeg() const {
    return argumentOfPeriapsis * 180.0 / M_PI;
}

double OrbitalElements::trueAnomalyDeg() const {
    return trueAnomaly * 180.0 / M_PI;
}

// Determine orbit type
std::string OrbitalElements::orbitType() const {
    if (eccentricity < 0.01) return "Circular";
    if (eccentricity < 1.0) return "Elliptical";
    if (std::abs(eccentricity - 1.0) < 0.01) return "Parabolic";
    return "Hyperbolic";
}

// Print all elements
void OrbitalElements::print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== Orbital Elements ===\n";
    std::cout << "Orbit Type: " << orbitType() << "\n";
    std::cout << "Semi-major Axis (a):  " << semiMajorAxis << " km\n";
    std::cout << "Eccentricity (e):     " << eccentricity << "\n";
    std::cout << "Inclination (i):      " << inclinationDeg() << " deg\n";
    std::cout << "RAAN (Ω):             " << raanDeg() << " deg\n";
    std::cout << "Arg of Periapsis (ω): " << argumentOfPeriapsisDeg() << " deg\n";
    std::cout << "True Anomaly (ν):     " << trueAnomalyDeg() << " deg\n";
    std::cout << "Periapsis:            " << periapsis << " km\n";
    std::cout << "Apoapsis:             " << apoapsis << " km\n";
    std::cout << "Period:               " << period / 60.0 << " min\n";
}
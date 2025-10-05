#include <iostream>
#include <cmath> 
#include "Vector3D.h"
#include "StateVector.h"

// Constants
const double MU_EARTH = 398600.4418;  // km^3/s^2
const double EARTH_RADIUS = 6378.137; // km

int main() {
    std::cout << "=== MDV - Phase 1 Test ===\n\n";
    
    // Test Vector3D
    std::cout << "Testing Vector3D class:\n";
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(4.0, 5.0, 6.0);
    
    std::cout << "v1 = "; v1.print(); std::cout << "\n";
    std::cout << "v2 = "; v2.print(); std::cout << "\n";
    
    Vector3D v3 = v1 + v2;
    std::cout << "v1 + v2 = "; v3.print(); std::cout << "\n";
    
    std::cout << "v1 · v2 = " << v1.dot(v2) << "\n";
    std::cout << "|v1| = " << v1.magnitude() << "\n\n";
    
    // Test StateVector - ISS orbit
    std::cout << "Testing StateVector - ISS Circular Orbit:\n";
    
    double altitude = 400.0;  // km
    double r = EARTH_RADIUS + altitude;
    double v_circular = std::pow(MU_EARTH / r, 0.5);
    
    std::cout << "Orbital radius: " << r << " km\n";
    std::cout << "Circular velocity: " << v_circular << " km/s\n\n";
    
    // Create initial state (circular orbit in equatorial plane)
    Vector3D pos(r, 0.0, 0.0);
    Vector3D vel(0.0, v_circular, 0.0);
    StateVector state(pos, vel, 0.0);
    
    state.print();
    
    std::cout << "\nOrbital energy: " << state.orbitalEnergy(MU_EARTH) << " km^2/s^2\n";
    std::cout << "Altitude: " << state.altitude(EARTH_RADIUS) << " km\n";
    
    Vector3D h = state.angularMomentum();
    std::cout << "Angular momentum: "; h.print(); std::cout << " km^2/s\n";
    std::cout << "|h| = " << h.magnitude() << " km^2/s\n";
    
    std::cout << "\n=== Tests Complete! ===\n";
    
    return 0;
}
#include <iostream>
#include <cmath>
#include <iomanip>
#include "Vector3D.h"
#include "StateVector.h"
#include "OrbitPropagator.h"

// Constants
const double MU_EARTH = 398600.4418;  // km^3/s^2
const double EARTH_RADIUS = 6378.137; // km

int main() {
    std::cout << "=== Mission Design Tool - Phase 1 Complete Test ===\n\n";
    
    // ISS circular orbit parameters
    double altitude = 400.0;  // km
    double r = EARTH_RADIUS + altitude;
    double v_circular = std::pow(MU_EARTH / r, 0.5);
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "ISS Orbit Parameters:\n";
    std::cout << "Altitude: " << altitude << " km\n";
    std::cout << "Orbital radius: " << r << " km\n";
    std::cout << "Circular velocity: " << v_circular << " km/s\n";
    
    // Calculate orbital period
    double period = 2.0 * M_PI * std::pow(r*r*r / MU_EARTH, 0.5);
    std::cout << "Orbital period: " << period/60.0 << " minutes\n\n";
    
    // Create initial state (circular orbit in equatorial plane)
    Vector3D pos(r, 0.0, 0.0);
    Vector3D vel(0.0, v_circular, 0.0);
    StateVector initialState(pos, vel, 0.0);
    
    // Create propagator
    OrbitPropagator propagator(MU_EARTH);
    
    std::cout << "=== Testing RK4 Integration ===\n\n";
    
    // Propagate for one complete orbit
    double timestep = 10.0;  // seconds
    std::vector<StateVector> trajectory = propagator.propagate(
        initialState, 
        period,  // one complete orbit
        timestep
    );
    
    std::cout << "Propagated " << trajectory.size() << " points over one orbit\n";
    std::cout << "Timestep: " << timestep << " seconds\n\n";
    
    // Check final state
    StateVector finalState = trajectory.back();
    
    std::cout << "Initial state:\n";
    initialState.print();
    
    std::cout << "\nFinal state (after one orbit):\n";
    finalState.print();
    
    // Calculate errors
    Vector3D posError = finalState.position - initialState.position;
    Vector3D velError = finalState.velocity - initialState.velocity;
    
    std::cout << "\n=== Orbit Closure Error ===\n";
    std::cout << "Position error: " << posError.magnitude() << " km\n";
    std::cout << "Velocity error: " << velError.magnitude() << " km/s\n";
    
    // Energy conservation
    double initialEnergy = initialState.orbitalEnergy(MU_EARTH);
    double finalEnergy = finalState.orbitalEnergy(MU_EARTH);
    double energyError = (finalEnergy - initialEnergy) / initialEnergy * 100.0;
    
    std::cout << "\n=== Energy Conservation ===\n";
    std::cout << "Initial energy: " << initialEnergy << " km²/s²\n";
    std::cout << "Final energy: " << finalEnergy << " km²/s²\n";
    std::cout << "Energy drift: " << energyError << " %\n";
    
    // Sample some points from the trajectory
    std::cout << "\n=== Sample Trajectory Points ===\n";
    for (size_t i = 0; i < trajectory.size(); i += trajectory.size()/8) {
        std::cout << "\nPoint " << i << " (t = " << trajectory[i].time << " s):\n";
        std::cout << "  r = " << trajectory[i].position.magnitude() << " km\n";
        std::cout << "  v = " << trajectory[i].speed() << " km/s\n";
        std::cout << "  altitude = " << trajectory[i].altitude(EARTH_RADIUS) << " km\n";
    }
    
    std::cout << "\n=== Tests Complete! ===\n";
    std::cout << "Phase 1 successfully completed.\n";
    std::cout << "Ready for Phase 2: 3D Visualization\n";
    
    return 0;
}
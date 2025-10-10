#include "OrbitPropagator.h"

OrbitPropagator::OrbitPropagator(double gravitationalParameter)
    : mu(gravitationalParameter) {
    // Default to RK4
    integrator = std::make_unique<RK4Integrator>();
}

// Propagate for a specified duration
std::vector<StateVector> OrbitPropagator::propagate(
    const StateVector& initialState,
    double duration,
    double timestep
) {
    std::vector<StateVector> trajectory;
    StateVector current = initialState;
    trajectory.push_back(current);
    
    int numSteps = static_cast<int>(duration / timestep);
    
    for (int i = 0; i < numSteps; ++i) {
        current = integrator->step(current, timestep, mu);
        trajectory.push_back(current);
    }
    
    return trajectory;
}

// Single step
StateVector OrbitPropagator::step(
    const StateVector& current,
    double timestep
) {
    return integrator->step(current, timestep, mu);
}

// Change integration method
void OrbitPropagator::setIntegrator(std::unique_ptr<Integrator> newIntegrator) {
    integrator = std::move(newIntegrator);
}
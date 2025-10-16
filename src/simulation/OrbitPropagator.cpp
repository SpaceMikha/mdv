#include "OrbitPropagator.h"

OrbitPropagator::OrbitPropagator(double gravitationalParameter)
    : mu(gravitationalParameter), forceModel() {  // Initialize with default force model
    // Default to RK4
    integrator = std::make_unique<RK4Integrator>();
}

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
        current = integrator->step(current, timestep, mu, forceModel);  // Pass force model
        trajectory.push_back(current);
    }
    
    return trajectory;
}

StateVector OrbitPropagator::step(
    const StateVector& current,
    double timestep
) {
    return integrator->step(current, timestep, mu, forceModel);  // Pass force model
}

void OrbitPropagator::setIntegrator(std::unique_ptr<Integrator> newIntegrator) {
    integrator = std::move(newIntegrator);
}
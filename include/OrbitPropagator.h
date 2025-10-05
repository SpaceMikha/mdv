#ifndef ORBITPROPAGATOR_H
#define ORBITPROPAGATOR_H

#include "StateVector.h"
#include "Integrator.h"
#include <memory>
#include <vector>

class OrbitPropagator {
private:
    std::unique_ptr<Integrator> integrator;
    double mu;  // Gravitational parameter
    
public:
    OrbitPropagator(double gravitationalParameter);
    
    // Propagate for a specified duration
    std::vector<StateVector> propagate(
        const StateVector& initialState,
        double duration,
        double timestep
    );
    
    // Single step
    StateVector step(
        const StateVector& current,
        double timestep
    );
    
    // Change integration method
    void setIntegrator(std::unique_ptr<Integrator> newIntegrator);
};

#endif // ORBITPROPAGATOR_H
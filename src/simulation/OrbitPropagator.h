#ifndef ORBITPROPAGATOR_H
#define ORBITPROPAGATOR_H

#include "StateVector.h"
#include "Integrator.h"
#include "ForceModel.h"
#include <memory>
#include <vector>

class OrbitPropagator {
private:
    std::unique_ptr<Integrator> integrator;
    double mu;
    ForceModel forceModel;
    
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

    void setForceModel(const ForceModel& model) { forceModel = model; }
    ForceModel& getForceModel() { return forceModel; }
    const ForceModel& getForceModel() const { return forceModel; }


};

#endif // ORBITPROPAGATOR_H
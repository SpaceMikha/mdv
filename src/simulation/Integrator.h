#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "StateVector.h"
#include "ForceModel.h"

class Integrator {
public:
    virtual ~Integrator() = default;
    
    // Pure virtual function 
    virtual StateVector step(
        const StateVector& current,
        double timestep,
        double mu,
        const ForceModel& forces
    ) const = 0;
    
protected:
    // Compute total acceleration from all forces
    Vector3D computeAcceleration(
        const Vector3D& position,
        double mu,
        const ForceModel& forces
    ) const;
    
    // Individual force calculations
    Vector3D computePointMassGravity(const Vector3D& position, double mu) const;
    Vector3D computeJ2Perturbation(const Vector3D& position, double mu) const;
};

// Euler integrator (simple, first-order)
class EulerIntegrator : public Integrator {
public:
    StateVector step(
        const StateVector& current,
        double timestep,
        double mu,
        const ForceModel& forces
    ) const override;
};

// Runge-Kutta 4th order (accurate, fourth-order)
class RK4Integrator : public Integrator {
public:
    StateVector step(
        const StateVector& current,
        double timestep,
        double mu,
        const ForceModel& forces
    ) const override;
};

#endif // INTEGRATOR_H
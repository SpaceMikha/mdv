#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "StateVector.h"

class Integrator {
public:
    virtual ~Integrator() = default;
    
    // Pure virtual function - must be implemented by derived classes
    virtual StateVector step(
        const StateVector& current,
        double timestep,
        double mu
    ) const = 0;
    
protected:
    // Acceleration function: a = -μ/r³ · r
    Vector3D computeAcceleration(
        const Vector3D& position,
        double mu
    ) const;
};

// Euler integrator (simple, first-order)
class EulerIntegrator : public Integrator {
public:
    StateVector step(
        const StateVector& current,
        double timestep,
        double mu
    ) const override;
};

// Runge-Kutta 4th order (accurate, fourth-order)
class RK4Integrator : public Integrator {
public:
    StateVector step(
        const StateVector& current,
        double timestep,
        double mu
    ) const override;
};

#endif // INTEGRATOR_H
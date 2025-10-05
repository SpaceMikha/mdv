#include "Integrator.h"
#include <cmath>

// Base class: compute two-body acceleration
Vector3D Integrator::computeAcceleration(
    const Vector3D& position,
    double mu
) const {
    double r = position.magnitude();
    double r3 = r * r * r;
    return position * (-mu / r3);
}

// ============================================================================
// EULER INTEGRATOR
// ============================================================================

StateVector EulerIntegrator::step(
    const StateVector& state,
    double h,
    double mu
) const {
    // Simple Euler: y(t+h) = y(t) + h*f(t,y)
    
    Vector3D acceleration = computeAcceleration(state.position, mu);
    
    Vector3D newPosition = state.position + state.velocity * h;
    Vector3D newVelocity = state.velocity + acceleration * h;
    
    return StateVector(newPosition, newVelocity, state.time + h);
}

// ============================================================================
// RK4 INTEGRATOR
// ============================================================================

StateVector RK4Integrator::step(
    const StateVector& state,
    double h,
    double mu
) const {
    // RK4: Fourth-order Runge-Kutta integration
    
    // k1 = f(t, y)
    Vector3D k1_v = state.velocity;
    Vector3D k1_a = computeAcceleration(state.position, mu);
    
    // k2 = f(t + h/2, y + h*k1/2)
    Vector3D pos2 = state.position + k1_v * (h/2.0);
    Vector3D vel2 = state.velocity + k1_a * (h/2.0);
    Vector3D k2_v = vel2;
    Vector3D k2_a = computeAcceleration(pos2, mu);
    
    // k3 = f(t + h/2, y + h*k2/2)
    Vector3D pos3 = state.position + k2_v * (h/2.0);
    Vector3D vel3 = state.velocity + k2_a * (h/2.0);
    Vector3D k3_v = vel3;
    Vector3D k3_a = computeAcceleration(pos3, mu);
    
    // k4 = f(t + h, y + h*k3)
    Vector3D pos4 = state.position + k3_v * h;
    Vector3D vel4 = state.velocity + k3_a * h;
    Vector3D k4_v = vel4;
    Vector3D k4_a = computeAcceleration(pos4, mu);
    
    // Weighted average
    Vector3D newPos = state.position + 
        (k1_v + k2_v*2.0 + k3_v*2.0 + k4_v) * (h/6.0);
    Vector3D newVel = state.velocity + 
        (k1_a + k2_a*2.0 + k3_a*2.0 + k4_a) * (h/6.0);
    
    return StateVector(newPos, newVel, state.time + h);
}
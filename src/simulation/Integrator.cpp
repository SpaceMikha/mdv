#include "Integrator.h"
#include "Constants.h"
#include <cmath>

// ============================================================================
// FORCE CALCULATIONS
// ============================================================================

Vector3D Integrator::computePointMassGravity(const Vector3D& position, double mu) const {
    double r = position.magnitude();
    double r3 = r * r * r;
    return position * (-mu / r3);
}

Vector3D Integrator::computeJ2Perturbation(const Vector3D& position, double mu) const {
    double r = position.magnitude();
    double x = position.x;
    double y = position.y;
    double z = position.z;
    
    double r2 = r * r;
    double z2 = z * z;
    
    // J2 perturbation acceleration
    double factor = (1.5 * EARTH_J2 * mu * EARTH_RADIUS * EARTH_RADIUS) / (r2 * r2 * r);
    
    double ax = x * factor * (5.0 * z2 / r2 - 1.0);
    double ay = y * factor * (5.0 * z2 / r2 - 1.0);
    double az = z * factor * (5.0 * z2 / r2 - 3.0);
    
    return Vector3D(ax, ay, az);
}

Vector3D Integrator::computeAcceleration(
    const Vector3D& position,
    double mu,
    const ForceModel& forces
) const {
    Vector3D totalAccel(0, 0, 0);
    
    // Point mass gravity (always included)
    if (forces.pointMass) {
        totalAccel += computePointMassGravity(position, mu);
    }
    
    // J2 perturbation
    if (forces.j2Perturbation) {
        totalAccel += computeJ2Perturbation(position, mu);
    }
    
    // Future: Add J3, J4, drag, SRP, third-body here
    
    return totalAccel;
}

// ============================================================================
// EULER INTEGRATOR
// ============================================================================

StateVector EulerIntegrator::step(
    const StateVector& state,
    double h,
    double mu,
    const ForceModel& forces
) const {
    // Simple Euler: y(t+h) = y(t) + h*f(t,y)
    
    Vector3D acceleration = computeAcceleration(state.position, mu, forces);
    
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
    double mu,
    const ForceModel& forces
) const {
    // RK4: Fourth-order Runge-Kutta integration
    
    // k1 = f(t, y)
    Vector3D k1_v = state.velocity;
    Vector3D k1_a = computeAcceleration(state.position, mu, forces);
    
    // k2 = f(t + h/2, y + h*k1/2)
    Vector3D pos2 = state.position + k1_v * (h/2.0);
    Vector3D vel2 = state.velocity + k1_a * (h/2.0);
    Vector3D k2_v = vel2;
    Vector3D k2_a = computeAcceleration(pos2, mu, forces);
    
    // k3 = f(t + h/2, y + h*k2/2)
    Vector3D pos3 = state.position + k2_v * (h/2.0);
    Vector3D vel3 = state.velocity + k2_a * (h/2.0);
    Vector3D k3_v = vel3;
    Vector3D k3_a = computeAcceleration(pos3, mu, forces);
    
    // k4 = f(t + h, y + h*k3)
    Vector3D pos4 = state.position + k3_v * h;
    Vector3D vel4 = state.velocity + k3_a * h;
    Vector3D k4_v = vel4;
    Vector3D k4_a = computeAcceleration(pos4, mu, forces);
    
    // Weighted average
    Vector3D newPos = state.position + 
        (k1_v + k2_v*2.0 + k3_v*2.0 + k4_v) * (h/6.0);
    Vector3D newVel = state.velocity + 
        (k1_a + k2_a*2.0 + k3_a*2.0 + k4_a) * (h/6.0);
    
    return StateVector(newPos, newVel, state.time + h);
}
#include "StateVector.h"
#include <iostream>
#include <iomanip>

// Constructors
StateVector::StateVector() 
    : position(Vector3D()), velocity(Vector3D()), time(0.0) {}

StateVector::StateVector(const Vector3D& pos, const Vector3D& vel, double t)
    : position(pos), velocity(vel), time(t) {}

// Orbital energy per unit mass
double StateVector::orbitalEnergy(double mu) const {
    double r = position.magnitude();
    double v = velocity.magnitude();
    return v*v/2.0 - mu/r;
}

// Specific angular momentum
Vector3D StateVector::angularMomentum() const {
    return position.cross(velocity);
}

// Altitude above body surface
double StateVector::altitude(double bodyRadius) const {
    return position.magnitude() - bodyRadius;
}

// Speed
double StateVector::speed() const {
    return velocity.magnitude();
}

// Print state
void StateVector::print() const {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Time: " << time << " s\n";
    std::cout << "Position: "; position.print(); std::cout << " km\n";
    std::cout << "Velocity: "; velocity.print(); std::cout << " km/s\n";
}
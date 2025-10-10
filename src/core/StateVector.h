#ifndef STATEVECTOR_H
#define STATEVECTOR_H

#include "Vector3D.h"

class StateVector {
public:
    Vector3D position;  // km
    Vector3D velocity;  // km/s
    double time;        // seconds since epoch
    
    // Constructors
    StateVector();
    StateVector(const Vector3D& pos, const Vector3D& vel, double t = 0.0);
    
    // Derived quantities
    double orbitalEnergy(double mu) const;
    Vector3D angularMomentum() const;
    double altitude(double bodyRadius) const;
    double speed() const;
    
    // Utility
    void print() const;
};

#endif // STATEVECTOR_H
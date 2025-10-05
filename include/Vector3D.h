#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>

class Vector3D {
public:
    double x, y, z;
    
    // Constructors
    Vector3D();
    Vector3D(double x, double y, double z);
    
    // Basic operations
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(double scalar) const;
    Vector3D operator/(double scalar) const;
    
    // Compound assignment operators
    Vector3D& operator+=(const Vector3D& other);
    Vector3D& operator-=(const Vector3D& other);
    
    // Vector operations
    double dot(const Vector3D& other) const;
    Vector3D cross(const Vector3D& other) const;
    double magnitude() const;
    double magnitudeSquared() const;
    Vector3D normalized() const;
    
    // Utility
    double distance(const Vector3D& other) const;
    void print() const;
};

#endif // VECTOR3D_H
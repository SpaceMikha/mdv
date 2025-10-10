#include "Vector3D.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using std::sqrt;
using std::cout;
using std::fixed;
using std::setprecision;

// Constructors
Vector3D::Vector3D() : x(0.0), y(0.0), z(0.0) {}

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

// Addition
Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

// Subtraction
Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

// Scalar multiplication
Vector3D Vector3D::operator*(double scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

// Scalar division
Vector3D Vector3D::operator/(double scalar) const {
    return Vector3D(x / scalar, y / scalar, z / scalar);
}

// Compound assignment +=
Vector3D& Vector3D::operator+=(const Vector3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

// Compound assignment -=
Vector3D& Vector3D::operator-=(const Vector3D& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

// Dot product
double Vector3D::dot(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

// Cross product
Vector3D Vector3D::cross(const Vector3D& other) const {
    return Vector3D(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

// Magnitude
double Vector3D::magnitude() const {
    return std::pow(x*x + y*y + z*z, 0.5);
}

// Magnitude squared (faster, no sqrt)
double Vector3D::magnitudeSquared() const {
    return x*x + y*y + z*z;
}

// Normalized vector
Vector3D Vector3D::normalized() const {
    double mag = magnitude();
    if (mag < 1e-10) {
        return Vector3D(0, 0, 0);
    }
    return *this / mag;
}

// Distance between two points
double Vector3D::distance(const Vector3D& other) const {
    return (*this - other).magnitude();
}

// Print vector
void Vector3D::print() const {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "[" << x << ", " << y << ", " << z << "]";
}
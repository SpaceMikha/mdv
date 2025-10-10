#include "Eclipse.h"
#include "Constants.h"
#include <cmath>

EclipseStatus EclipseDetector::checkEclipse(
    const Vector3D& satPos,
    const Vector3D& sunDir,
    double earthRadius
) {
    EclipseStatus status;
    
    // Sun direction should be normalized
    Vector3D sunNorm = sunDir.normalized();
    
    // Satellite position vector from Earth center
    Vector3D satNorm = satPos.normalized();
    double satDistance = satPos.magnitude();
    
    // Angle between sun and satellite (from Earth center)
    double cosAngle = satNorm.dot(sunNorm);
    status.sunAngle = acos(cosAngle) * 180.0 / M_PI;
    
    // If satellite is on sun side, no eclipse
    if (cosAngle > 0) {
        return status;
    }
    
    // Calculate angular radius of Earth from satellite
    double earthAngularRadius = asin(earthRadius / satDistance);
    
    // Angle from sun direction to satellite
    double angleFromSun = acos(-cosAngle);  // Negative because satellite is behind Earth
    
    // Check umbra (full shadow)
    if (angleFromSun < (earthAngularRadius - SUN_ANGULAR_RADIUS)) {
        status.inUmbra = true;
        status.inPenumbra = true;
    }
    // Check penumbra (partial shadow)
    else if (angleFromSun < (earthAngularRadius + SUN_ANGULAR_RADIUS)) {
        status.inPenumbra = true;
    }
    
    return status;
}
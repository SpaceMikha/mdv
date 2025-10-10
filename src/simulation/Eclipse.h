#ifndef ECLIPSE_H
#define ECLIPSE_H

#include "Vector3D.h"

// Eclipse status structure
struct EclipseStatus {
    bool inUmbra;        // Full shadow
    bool inPenumbra;     // Partial shadow
    double sunAngle;     // Angle between sun and satellite from Earth center (degrees)
    
    EclipseStatus() : inUmbra(false), inPenumbra(false), sunAngle(0.0) {}
};

// Eclipse detection class
class EclipseDetector {
public:
    // Check if satellite is in Earth's shadow
    static EclipseStatus checkEclipse(
        const Vector3D& satPos,
        const Vector3D& sunDir,
        double earthRadius
    );
};

#endif // ECLIPSE_H
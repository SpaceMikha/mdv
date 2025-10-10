#ifndef GRID_RENDERER_H
#define GRID_RENDERER_H

#include "Vector3D.h"
#include "StateVector.h"
#include "raylib.h"
#include <vector>

// Grid and reference line rendering
class GridRenderer {
public:
    // Draw equatorial grid
    static void drawEquatorialGrid();
    
    // Draw reference altitude circles (LEO, GEO)
    static void drawReferenceCircles();
    
    // Draw orbital plane for a specific orbit
    static void drawOrbitalPlane(const std::vector<StateVector>& orbit, Color color);
    
    // Draw coordinate axes
    static void drawCoordinateAxes(float length = 10.0f);
};

#endif // GRID_RENDERER_H
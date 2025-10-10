#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include "Vector3D.h"
#include "raylib.h"

// Rendering utility functions
class RenderUtils {
public:
    // Convert Vector3D to Raylib Vector3
    static Vector3 toRaylib(const Vector3D& v);
    
    // Convert km to rendering units
    static float toRenderScale(double km);
};

#endif // RENDER_UTILS_H
#include "RenderUtils.h"
#include "Constants.h"

Vector3 RenderUtils::toRaylib(const Vector3D& v) {
    return Vector3{
        static_cast<float>(v.x * SCALE),
        static_cast<float>(v.y * SCALE),
        static_cast<float>(v.z * SCALE)
    };
}

float RenderUtils::toRenderScale(double km) {
    return static_cast<float>(km * SCALE);
}
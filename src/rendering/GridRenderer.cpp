#include "GridRenderer.h"
#include "RenderUtils.h"
#include "Constants.h"
#include <cmath>

void GridRenderer::drawEquatorialGrid() {
    float spacing = GRID_SPACING;
    int numLines = GRID_LINES;
    
    for (int i = -numLines; i <= numLines; i++) {
        float z = i * spacing;
        float distFromCenter = fabs((float)i) / (float)numLines;
        float alpha = (1.0f - distFromCenter * distFromCenter) * 0.4f;
        Color lineColor = Fade(GRAY, alpha);
        
        if (i == 0) {
            lineColor = Fade(SKYBLUE, 0.3f);
        }
        
        DrawLine3D(
            Vector3{-numLines * spacing, 0.01f, z},
            Vector3{numLines * spacing, 0.01f, z},
            lineColor
        );
    }
    
    for (int i = -numLines; i <= numLines; i++) {
        float x = i * spacing;
        float distFromCenter = fabs((float)i) / (float)numLines;
        float alpha = (1.0f - distFromCenter * distFromCenter) * 0.4f;
        Color lineColor = Fade(GRAY, alpha);
        
        if (i == 0) {
            lineColor = Fade(RED, 0.3f);
        }
        
        DrawLine3D(
            Vector3{x, 0.01f, -numLines * spacing},
            Vector3{x, 0.01f, numLines * spacing},
            lineColor
        );
    }
}

void GridRenderer::drawReferenceCircles() {
    float leoRadius = (EARTH_RADIUS + LEO_MAX_ALTITUDE) * SCALE;
    Color leoColor = Fade(Color{100, 200, 255, 255}, 0.3f);
    DrawCircle3D(Vector3{0, 0, 0}, leoRadius, Vector3{1, 0, 0}, 90.0f, leoColor);
    
    float geoRadius = (EARTH_RADIUS + GEO_ALTITUDE) * SCALE;
    Color geoColor = Fade(Color{255, 100, 255, 255}, 0.3f);
    DrawCircle3D(Vector3{0, 0, 0}, geoRadius, Vector3{1, 0, 0}, 90.0f, geoColor);
}

void GridRenderer::drawOrbitalPlane(const std::vector<StateVector>& orbit, Color color) {
    if (orbit.size() < 3) return;
    
    Vector3D r = orbit[0].position;
    Vector3D v = orbit[0].velocity;
    Vector3D h = r.cross(v);
    h = h.normalized();
    
    double maxR = 0;
    for (const auto& state : orbit) {
        double r = state.position.magnitude();
        if (r > maxR) maxR = r;
    }
    
    float planeSize = maxR * SCALE * 1.2f;
    
    int gridLines = 10;
    float step = planeSize * 2.0f / gridLines;
    
    Vector3D up(0, 1, 0);
    Vector3D right = h.cross(up).normalized();
    Vector3D forward = right.cross(h).normalized();
    
    Color lineColor = Fade(color, 0.25f);
    
    for (int i = -gridLines/2; i <= gridLines/2; i++) {
        float offset = i * step;
        
        Vector3D start1 = right * (-planeSize) + forward * offset;
        Vector3D end1 = right * planeSize + forward * offset;
        DrawLine3D(RenderUtils::toRaylib(start1), RenderUtils::toRaylib(end1), lineColor);
        
        Vector3D start2 = forward * (-planeSize) + right * offset;
        Vector3D end2 = forward * planeSize + right * offset;
        DrawLine3D(RenderUtils::toRaylib(start2), RenderUtils::toRaylib(end2), lineColor);
    }
}

void GridRenderer::drawCoordinateAxes(float length) {
    DrawLine3D(Vector3{0, 0, 0}, Vector3{length, 0, 0}, RED);
    DrawLine3D(Vector3{0, 0, 0}, Vector3{0, length, 0}, GREEN);
    DrawLine3D(Vector3{0, 0, 0}, Vector3{0, 0, length}, SKYBLUE);
}
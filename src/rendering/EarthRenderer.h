#ifndef EARTH_RENDERER_H
#define EARTH_RENDERER_H

#include "raylib.h"

// Earth rendering system
class EarthRenderer {
public:
    EarthRenderer();
    ~EarthRenderer();
    
    // Initialize Earth model and texture
    void load();
    
    // Update rotation
    void update(float deltaTime);
    
    // Render Earth
    void draw() const;
    
    // Cleanup
    void unload();
    
    // Control rotation
    void setRotationEnabled(bool enabled) { rotationEnabled = enabled; }
    bool isRotationEnabled() const { return rotationEnabled; }
    
private:
    Model earthModel;
    Texture2D earthTexture;
    bool textureLoaded;
    bool rotationEnabled;
    float rotationAngle;
};

#endif // EARTH_RENDERER_H
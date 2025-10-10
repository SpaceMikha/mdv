#ifndef FONT_SYSTEM_H
#define FONT_SYSTEM_H

#include "raylib.h"

// Font management system
class FontSystem {
public:
    FontSystem();
    ~FontSystem();
    
    // Load fonts
    void load();
    
    // Cleanup
    void unload();
    
    // Draw text with font
    void drawText(const char* text, float x, float y, float fontSize, Color color, bool bold = false) const;
    
    // Measure text
    Vector2 measureText(const char* text, float fontSize) const;
    
    // Check if loaded
    bool isLoaded() const { return loaded; }
    
private:
    Font regular;
    Font bold;
    bool loaded;
};

#endif // FONT_SYSTEM_H
#include "FontSystem.h"
#include <iostream>

FontSystem::FontSystem() : loaded(false) {}

FontSystem::~FontSystem() {
    unload();
}

void FontSystem::load() {
    std::cout << "Loading fonts...\n";
    
    regular = LoadFontEx("../assets/fonts/Roboto-Regular.ttf", 72, 0, 250);
    bold = LoadFontEx("../assets/fonts/Roboto-Bold.ttf", 72, 0, 250);
    
    if (regular.texture.id == 0) {
        std::cout << "  Roboto not found, trying Segoe UI...\n";
        regular = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 72, 0, 250);
        if (regular.texture.id == 0) {
            std::cout << "  Using default font\n";
            regular = GetFontDefault();
        } else {
            std::cout << "  Segoe UI loaded!\n";
        }
    } else {
        std::cout << "  Roboto-Regular.ttf loaded!\n";
    }
    
    if (bold.texture.id == 0) {
        std::cout << "  Roboto Bold not found, trying Segoe UI Bold...\n";
        bold = LoadFontEx("C:/Windows/Fonts/segoeuib.ttf", 72, 0, 250);
        if (bold.texture.id == 0) {
            std::cout << "  Using default font\n";
            bold = GetFontDefault();
        } else {
            std::cout << "  Segoe UI Bold loaded!\n";
        }
    } else {
        std::cout << "  Roboto-Bold.ttf loaded!\n";
    }
    
    if (regular.texture.id != 1) {
        SetTextureFilter(regular.texture, TEXTURE_FILTER_BILINEAR);
    }
    if (bold.texture.id != 1) {
        SetTextureFilter(bold.texture, TEXTURE_FILTER_BILINEAR);
    }
    
    loaded = true;
}

void FontSystem::unload() {
    if (loaded) {
        if (regular.texture.id != 1) UnloadFont(regular);
        if (bold.texture.id != 1) UnloadFont(bold);
        loaded = false;
    }
}

void FontSystem::drawText(const char* text, float x, float y, float fontSize, Color color, bool bold_flag) const {
    Font font = bold_flag ? bold : regular;
    Vector2 position = { x, y };
    float spacing = 0.0f;
    DrawTextEx(font, text, position, fontSize, spacing, color);
}

Vector2 FontSystem::measureText(const char* text, float fontSize) const {
    return MeasureTextEx(regular, text, fontSize, 0.0f);
}
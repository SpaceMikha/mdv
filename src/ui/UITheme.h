#ifndef UI_THEME_H
#define UI_THEME_H

#include "raylib.h"

// Color Pallete
namespace UITheme {
    // Primary Colors
    inline const Color PRIMARY = Color{30, 58, 138, 255};      // Deep Blue #1E3A8A
    inline const Color SECONDARY = Color{59, 130, 246, 255};   // Bright Blue #3B82F6
    inline const Color ACCENT = Color{16, 185, 129, 255};      // Green #10B981
    inline const Color PREALPHA_ACCENT = Color{236, 72, 153, 255}; // Pink #EC4899 
    
    // Status Colors
    inline const Color SUCCESS = Color{16, 185, 129, 255};     // Green #10B981
    inline const Color WARNING = Color{245, 158, 11, 255};     // Amber #F59E0B
    inline const Color DANGER = Color{239, 68, 68, 255};       // Red #EF4444
    inline const Color INFO = Color{59, 130, 246, 255};        // Blue #3B82F6
    
    // Text Colors
    inline const Color TEXT_PRIMARY = Color{243, 244, 246, 255};    // Off-white #F3F4F6
    inline const Color TEXT_SECONDARY = Color{156, 163, 175, 255};  // Light gray #9CA3AF
    inline const Color TEXT_MUTED = Color{107, 114, 128, 255};      // Medium gray #6B7280
    
    // Background Colors
    inline const Color BG_DARK = Color{15, 23, 42, 255};       // Very dark blue #0F172A
    inline const Color BG_PANEL = Color{30, 41, 59, 255};      // Dark blue-gray #1E293B
    inline const Color BG_PANEL_HOVER = Color{51, 65, 85, 255}; // Medium blue-gray #334155
    
    // Border Colors
    inline const Color BORDER = Color{51, 65, 85, 255};        // Medium blue-gray #334155
    inline const Color BORDER_LIGHT = Color{71, 85, 105, 255}; // Lighter blue-gray #475569
    inline const Color BORDER_ACCENT = Color{59, 130, 246, 255}; // Bright blue
    
    // Orbit Family Colors (Keep existing for backward compatibility)
    inline const Color ORBIT_LEO = Color{100, 200, 255, 255};  // Light blue
    inline const Color ORBIT_MEO = Color{100, 255, 100, 255};  // Light green
    inline const Color ORBIT_HEO = Color{255, 150, 100, 255};  // Orange
    inline const Color ORBIT_GEO = Color{255, 100, 255, 255};  // Magenta
    
    // Typography Scale (pixel sizes)
    inline const float FONT_SIZE_TITLE = 28.0f;       // Main title
    inline const float FONT_SIZE_H1 = 22.0f;          // Panel headers
    inline const float FONT_SIZE_H2 = 18.0f;          // Section headers
    inline const float FONT_SIZE_H3 = 16.0f;          // Subsection headers
    inline const float FONT_SIZE_BODY = 14.0f;        // Body text
    inline const float FONT_SIZE_SMALL = 12.0f;       // Small text
    inline const float FONT_SIZE_TINY = 10.0f;        // Tiny text
    
    // Spacing Scale
    inline const int SPACING_XS = 4;
    inline const int SPACING_SM = 8;
    inline const int SPACING_MD = 12;
    inline const int SPACING_LG = 16;
    inline const int SPACING_XL = 24;
    inline const int SPACING_2XL = 32;
    
    // Panel Dimensions
    inline const int SIDEBAR_WIDTH = 320;
    inline const int TITLE_BAR_HEIGHT = 50;
    inline const int STATUS_BAR_HEIGHT = 35;
    inline const int PANEL_PADDING = 16;
    inline const int PANEL_BORDER_WIDTH = 2;
    
    // Opacity/Alpha values
    inline const float ALPHA_PANEL = 0.95f;
    inline const float ALPHA_HOVER = 0.85f;
    inline const float ALPHA_DISABLED = 0.5f;
    
    // Helper function: Draw panel with consistent styling
    inline void DrawPanel(int x, int y, int width, int height, Color borderColor = BORDER_ACCENT) {
        DrawRectangle(x, y, width, height, Fade(BG_PANEL, ALPHA_PANEL));
        DrawRectangleLines(x, y, width, height, borderColor);
    }
    
    // Helper function: Draw section divider
    inline void DrawDivider(int x, int y, int width) {
        DrawLineEx(
            Vector2{(float)x, (float)y}, 
            Vector2{(float)(x + width), (float)y}, 
            1.0f, 
            BORDER
        );
    }
}

// Include FontSystem after namespace declaration
#include "FontSystem.h"

// Now we can implement DrawBadge with full FontSystem definition
namespace UITheme {
    // Helper function: Draw badge/tag
    inline void DrawBadge(int x, int y, int width, int height, const char* text, 
                          Color bgColor, Color textColor, const FontSystem& fonts) {
        DrawRectangle(x, y, width, height, bgColor);
        DrawRectangleLines(x, y, width, height, BORDER);
        
        // Center text in badge
        Vector2 textSize = fonts.measureText(text, FONT_SIZE_BODY);
        int textX = x + (width - (int)textSize.x) / 2;
        int textY = y + (height - 14) / 2;
        
        fonts.drawText(text, (float)textX, (float)textY, FONT_SIZE_BODY, textColor, true);
    }
}

#endif // UI_THEME_H
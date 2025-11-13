#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct Point {
    float x, y;
};

struct Rect {
    float x, y, width, height;
    
    Rect(float x = 0, float y = 0, float w = 0, float h = 0)
        : x(x), y(y), width(w), height(h) {}
    
    bool contains_point(const Point& p) const {
        return p.x >= x && p.x < x + width &&
               p.y >= y && p.y < y + height;
    }
};

struct Color {
    uint8_t r, g, b, a;

    // constexpr constructor so Color is a literal type and can be used in constexpr variables
    constexpr Color(uint8_t rr = 0, uint8_t gg = 0, uint8_t bb = 0, uint8_t aa = 255) noexcept
        : r(rr), g(gg), b(bb), a(aa) {}

    static constexpr Color from_hex(uint32_t hex) noexcept {
        return Color(
            static_cast<uint8_t>((hex >> 16) & 0xFF),
            static_cast<uint8_t>((hex >> 8) & 0xFF),
            static_cast<uint8_t>(hex & 0xFF)
        );
    }
};

// Falkon macOS-inspired theme colors
namespace Theme {
    constexpr Color TOOLBAR_BG = Color(240, 240, 240);
    constexpr Color TAB_BG_INACTIVE = Color(230, 230, 230);
    constexpr Color TAB_BG_ACTIVE = Color(255, 255, 255);
    constexpr Color TAB_TEXT = Color(60, 60, 60);
    constexpr Color BUTTON_BG = Color(220, 220, 220);
    constexpr Color BUTTON_HOVER = Color(200, 200, 200);
    constexpr Color URLBAR_BG = Color(250, 250, 250);
    constexpr Color URLBAR_BORDER = Color(200, 200, 200);
    constexpr Color TEXT_PRIMARY = Color(0, 0, 0);
    constexpr Color TEXT_SECONDARY = Color(100, 100, 100);
    constexpr Color SEPARATOR = Color(200, 200, 200);
}

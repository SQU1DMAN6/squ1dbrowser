#include "ui_renderer.h"
#include "../../shared/bitmap_font.h"
#include <algorithm>
#include <cmath>

UIRenderer::UIRenderer(int width, int height)
    : width(width), height(height) {
    // Initialize frame buffer (RGBA)
    frame_buffer.resize(width * height * 4);
    clear(Color(255, 255, 255)); // White background
}

UIRenderer::~UIRenderer() {}

void UIRenderer::clear(const Color& color) {
    for (int i = 0; i < width * height; ++i) {
        frame_buffer[i * 4 + 0] = color.r;
        frame_buffer[i * 4 + 1] = color.g;
        frame_buffer[i * 4 + 2] = color.b;
        frame_buffer[i * 4 + 3] = color.a;
    }
}

void UIRenderer::put_pixel(int x, int y, const Color& color) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    
    int idx = (y * width + x) * 4;
    frame_buffer[idx + 0] = color.r;
    frame_buffer[idx + 1] = color.g;
    frame_buffer[idx + 2] = color.b;
    frame_buffer[idx + 3] = color.a;
}

void UIRenderer::fill_rect(const Rect& rect, const Color& color) {
    fill_rect_internal(
        static_cast<int>(rect.x),
        static_cast<int>(rect.y),
        static_cast<int>(rect.x + rect.width),
        static_cast<int>(rect.y + rect.height),
        color
    );
}

void UIRenderer::fill_rect_internal(int x1, int y1, int x2, int y2, const Color& color) {
    x1 = std::max(0, x1);
    y1 = std::max(0, y1);
    x2 = std::min(width, x2);
    y2 = std::min(height, y2);
    
    for (int y = y1; y < y2; ++y) {
        for (int x = x1; x < x2; ++x) {
            put_pixel(x, y, color);
        }
    }
}

void UIRenderer::draw_rect(const Rect& rect, const Color& color, float stroke_width) {
    int sw = static_cast<int>(stroke_width);
    
    // Top edge
    fill_rect_internal(
        static_cast<int>(rect.x),
        static_cast<int>(rect.y),
        static_cast<int>(rect.x + rect.width),
        static_cast<int>(rect.y + sw),
        color
    );
    
    // Bottom edge
    fill_rect_internal(
        static_cast<int>(rect.x),
        static_cast<int>(rect.y + rect.height - sw),
        static_cast<int>(rect.x + rect.width),
        static_cast<int>(rect.y + rect.height),
        color
    );
    
    // Left edge
    fill_rect_internal(
        static_cast<int>(rect.x),
        static_cast<int>(rect.y),
        static_cast<int>(rect.x + sw),
        static_cast<int>(rect.y + rect.height),
        color
    );
    
    // Right edge
    fill_rect_internal(
        static_cast<int>(rect.x + rect.width - sw),
        static_cast<int>(rect.y),
        static_cast<int>(rect.x + rect.width),
        static_cast<int>(rect.y + rect.height),
        color
    );
}

void UIRenderer::draw_rounded_rect(const Rect& rect, const Color& color, float radius, float stroke_width) {
    // Simplified rounded rectangle - draw corners
    int r = static_cast<int>(radius);
    
    // Draw main rectangle
    fill_rect_internal(
        static_cast<int>(rect.x + r),
        static_cast<int>(rect.y),
        static_cast<int>(rect.x + rect.width - r),
        static_cast<int>(rect.y + rect.height),
        color
    );
    
    // Left and right columns
    fill_rect_internal(
        static_cast<int>(rect.x),
        static_cast<int>(rect.y + r),
        static_cast<int>(rect.x + rect.width),
        static_cast<int>(rect.y + rect.height - r),
        color
    );
}

void UIRenderer::draw_text(const std::string& text, float x, float y, const Color& color, float font_size) {
    // Use bitmap font for text rendering
    int px = static_cast<int>(x);
    int py = static_cast<int>(y);
    
    for (char c : text) {
        draw_char(frame_buffer.data(), width, height, px, py, c, color.r, color.g, color.b);
        px += 4; // 3 pixels for char + 1 pixel spacing
    }
}

void UIRenderer::draw_toolbar(int height) {
    Rect toolbar_rect(0, 0, width, height);
    fill_rect(toolbar_rect, Theme::TOOLBAR_BG);
    draw_rect(toolbar_rect, Theme::SEPARATOR, 1.0f);
}

void UIRenderer::draw_tab(const Rect& rect, const std::string& title, bool is_active) {
    // Draw tab background
    Color bg = is_active ? Theme::TAB_BG_ACTIVE : Theme::TAB_BG_INACTIVE;
    fill_rect(rect, bg);
    
    // Draw tab border with rounded corners
    draw_rounded_rect(rect, Theme::SEPARATOR, 4.0f, 1.0f);
    
    // Draw tab text
    draw_text(title, rect.x + 10, rect.y + 5, Theme::TAB_TEXT, 11.0f);
}

void UIRenderer::draw_url_bar(const Rect& rect, const std::string& url, bool focused) {
    fill_rect(rect, Theme::URLBAR_BG);
    
    Color border = focused ? Color(100, 150, 255) : Theme::URLBAR_BORDER;
    draw_rounded_rect(rect, border, 6.0f, 1.0f);
    
    draw_text(url, rect.x + 8, rect.y + 8, Theme::TEXT_PRIMARY, 12.0f);
}

void UIRenderer::draw_button(const Rect& rect, const std::string& label, bool hovered) {
    Color bg = hovered ? Theme::BUTTON_HOVER : Theme::BUTTON_BG;
    fill_rect(rect, bg);
    draw_rounded_rect(rect, Theme::SEPARATOR, 4.0f, 1.0f);
    
    draw_text(label, rect.x + 5, rect.y + 5, Theme::TEXT_PRIMARY, 11.0f);
}

void UIRenderer::draw_line(float x1, float y1, float x2, float y2, const Color& color) {
    // Bresenham line algorithm (simplified)
    int dx = static_cast<int>(x2 - x1);
    int dy = static_cast<int>(y2 - y1);
    
    int steps = std::max(std::abs(dx), std::abs(dy));
    if (steps == 0) return;
    
    float x_inc = dx / static_cast<float>(steps);
    float y_inc = dy / static_cast<float>(steps);
    
    for (int i = 0; i <= steps; ++i) {
        put_pixel(
            static_cast<int>(x1 + i * x_inc),
            static_cast<int>(y1 + i * y_inc),
            color
        );
    }
}

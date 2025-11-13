#pragma once

#include "ui_types.h"
#include <cstdint>
#include <vector>

class UIRenderer {
public:
    UIRenderer(int width, int height);
    ~UIRenderer();

    // Drawing primitives
    void clear(const Color& color);
    void fill_rect(const Rect& rect, const Color& color);
    void draw_rect(const Rect& rect, const Color& color, float stroke_width = 1.0f);
    void draw_rounded_rect(const Rect& rect, const Color& color, float radius, float stroke_width = 1.0f);
    void draw_text(const std::string& text, float x, float y, const Color& color, float font_size = 12.0f);
    
    // High-level UI elements (Falkon macOS inspired)
    void draw_toolbar(int height);
    void draw_tab(const Rect& rect, const std::string& title, bool is_active);
    void draw_url_bar(const Rect& rect, const std::string& url, bool focused);
    void draw_button(const Rect& rect, const std::string& label, bool hovered = false);
    
    // Get rendered frame
    const std::vector<uint8_t>& get_frame_buffer() const { return frame_buffer; }
    
    int get_width() const { return width; }
    int get_height() const { return height; }

private:
    int width, height;
    std::vector<uint8_t> frame_buffer; // RGBA format
    
    // Helper methods
    void put_pixel(int x, int y, const Color& color);
    void draw_line(float x1, float y1, float x2, float y2, const Color& color);
    void fill_rect_internal(int x1, int y1, int x2, int y2, const Color& color);
};

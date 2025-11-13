#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include "tab_manager.h"
#include "ui_renderer.h"
#include "renderer_bridge.h"

struct SDL_Window;
struct SDL_Surface;

class BrowserWindow {
public:
    BrowserWindow(int width, int height, const std::string& title);
    ~BrowserWindow();
    
    // Main window loop
    void run();
    
    // Event handling
    void handle_events();
    void handle_mouse_click(float x, float y);
    void handle_key_press(int key);
    
    // Navigation
    void navigate_to(const std::string& url);
    void go_back();
    void go_forward();
    void refresh();
    
    // UI Updates
    void update_display();
    
    bool is_running() const { return running; }

private:
    // Window and rendering
    SDL_Window* window;
    SDL_Surface* surface;
    int window_width, window_height;
    bool running;
    
    // Browser components
    std::unique_ptr<TabManager> tab_manager;
    std::unique_ptr<UIRenderer> ui_renderer;
    std::unique_ptr<RendererBridge> renderer_bridge;
    
    // History for back/forward (simplified)
    std::vector<std::string> history;
    int history_index;
    
    // UI State
    std::string current_url;
    bool url_bar_focused;
    
    // Helper methods
    void render_frame();
    void update_url_bar_from_input(const std::string& input);
};

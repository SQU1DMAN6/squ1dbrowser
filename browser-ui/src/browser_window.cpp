#include "browser_window.h"
#include <SDL2/SDL.h>
#include <iostream>

BrowserWindow::BrowserWindow(int width, int height, const std::string& title)
    : window_width(width), window_height(height), running(true),
      history_index(0), url_bar_focused(false) {
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }
    
    // Create window
    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        running = false;
        return;
    }
    
    surface = SDL_GetWindowSurface(window);
    
    // Initialize components
    tab_manager = std::make_unique<TabManager>();
    ui_renderer = std::make_unique<UIRenderer>(width, height);
    renderer_bridge = std::make_unique<RendererBridge>();
    
    current_url = "https://google.com";
}

BrowserWindow::~BrowserWindow() {
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void BrowserWindow::run() {
    while (running) {
        handle_events();
        update_display();
        render_frame();
        SDL_Delay(16); // ~60 FPS
    }
}

void BrowserWindow::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    handle_mouse_click(event.button.x, event.button.y);
                }
                break;
            
            case SDL_KEYDOWN:
                handle_key_press(event.key.keysym.sym);
                break;
            
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window_width = event.window.data1;
                    window_height = event.window.data2;
                    ui_renderer = std::make_unique<UIRenderer>(window_width, window_height);
                }
                break;
        }
    }
}

void BrowserWindow::handle_mouse_click(float x, float y) {
    // Check if clicking on toolbar buttons or tabs
    // Back button: (10, 10) to (45, 40)
    if (x >= 10 && x < 45 && y >= 10 && y < 40) {
        go_back();
        return;
    }
    
    // Forward button: (50, 10) to (85, 40)
    if (x >= 50 && x < 85 && y >= 10 && y < 40) {
        go_forward();
        return;
    }
    
    // Refresh button: (90, 10) to (125, 40)
    if (x >= 90 && x < 125 && y >= 10 && y < 40) {
        refresh();
        return;
    }
    
    // URL bar: (130, 10) to (800-60, 40)
    if (x >= 130 && x < window_width - 60 && y >= 10 && y < 40) {
        url_bar_focused = true;
        return;
    }
    
    // New tab button: (window_width - 50, 10) to (window_width - 10, 40)
    if (x >= window_width - 50 && x < window_width - 10 && y >= 10 && y < 40) {
        auto new_tab = tab_manager->create_tab("https://google.com");
        tab_manager->switch_tab(tab_manager->get_tab_count() - 1);
        return;
    }
}

void BrowserWindow::handle_key_press(int key) {
    switch (key) {
        case SDLK_ESCAPE:
            url_bar_focused = false;
            break;
        
        case SDLK_RETURN:
            if (url_bar_focused) {
                navigate_to(current_url);
                url_bar_focused = false;
            }
            break;
        
        case SDLK_w:
            // Cmd+W to close tab (would need modifier detection)
            if (tab_manager->get_tab_count() > 0) {
                tab_manager->close_tab(tab_manager->get_active_index());
            }
            break;
    }
}

void BrowserWindow::navigate_to(const std::string& url) {
    current_url = url;
    history.push_back(url);
    history_index = history.size() - 1;
    
    auto active_tab = tab_manager->get_active_tab();
    if (active_tab) {
        active_tab->url = url;
        active_tab->set_title("Loading...");
        active_tab->rendered_content.clear(); // Force re-render
        
        // Request render from bridge with delay consideration
        int content_width = window_width - 20;
        int content_height = window_height - 95;
        renderer_bridge->request_render(url, content_width, content_height);
        
        // Load the rendered content immediately
        auto rendered = renderer_bridge->get_rendered_content();
        if (!rendered.empty()) {
            active_tab->set_content(rendered);
            active_tab->set_title(url); // Update title once rendered
        }
    }
}

void BrowserWindow::go_back() {
    if (history_index > 0) {
        history_index--;
        current_url = history[history_index];
        navigate_to(current_url);
    }
}

void BrowserWindow::go_forward() {
    if (history_index < static_cast<int>(history.size()) - 1) {
        history_index++;
        current_url = history[history_index];
        navigate_to(current_url);
    }
}

void BrowserWindow::refresh() {
    navigate_to(current_url);
}

void BrowserWindow::update_display() {
    // Clear and redraw everything
    ui_renderer->clear(Color(255, 255, 255));
    
    // Draw toolbar
    ui_renderer->draw_toolbar(50);
    
    // Draw navigation buttons
    Rect back_btn(10, 10, 30, 30);
    ui_renderer->draw_button(back_btn, "←");
    
    Rect forward_btn(50, 10, 30, 30);
    ui_renderer->draw_button(forward_btn, "→");
    
    Rect refresh_btn(90, 10, 30, 30);
    ui_renderer->draw_button(refresh_btn, "⟳");
    
    // Draw URL bar
    Rect url_bar(130, 10, window_width - 190, 30);
    ui_renderer->draw_url_bar(url_bar, current_url, url_bar_focused);
    
    // Draw new tab button
    Rect new_tab_btn(window_width - 50, 10, 40, 30);
    ui_renderer->draw_button(new_tab_btn, "+");
    
    // Draw tabs
    float tab_x = 10;
    for (int i = 0; i < tab_manager->get_tab_count(); ++i) {
        auto tab = tab_manager->get_tab(i);
        if (tab) {
            Rect tab_rect(tab_x, 55, 100, 25);
            ui_renderer->draw_tab(tab_rect, tab->title, tab->is_active);
            tab_x += 105;
        }
    }
}

void BrowserWindow::render_frame() {
    if (!surface) return;
    
    // Copy frame buffer to SDL surface
    const auto& frame_buffer = ui_renderer->get_frame_buffer();
    
    // Lock surface
    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }
    
    // Copy pixel data from UI renderer
    uint32_t* pixels = static_cast<uint32_t*>(surface->pixels);
    for (size_t i = 0; i < frame_buffer.size(); i += 4) {
        uint32_t r = frame_buffer[i];
        uint32_t g = frame_buffer[i + 1];
        uint32_t b = frame_buffer[i + 2];
        uint32_t a = frame_buffer[i + 3];
        pixels[i / 4] = SDL_MapRGBA(surface->format, r, g, b, a);
    }
    
    // Blend rendered content from active tab if available
    auto active_tab = tab_manager->get_active_tab();
    if (active_tab && !active_tab->rendered_content.empty()) {
        int content_x = 10;
        int content_y = 85;
        int content_width = window_width - 20;
        int content_height = window_height - 95;
        
        if (content_width > 0 && content_height > 0) {
            const auto& rendered = active_tab->rendered_content;
            for (int y = 0; y < content_height && y * content_width * 4 + 3 < rendered.size(); ++y) {
                for (int x = 0; x < content_width; ++x) {
                    int src_idx = (y * content_width + x) * 4;
                    int dst_pixel_idx = (content_y + y) * window_width + (content_x + x);
                    
                    if (src_idx + 3 < rendered.size() && dst_pixel_idx < window_width * window_height) {
                        uint32_t r = rendered[src_idx];
                        uint32_t g = rendered[src_idx + 1];
                        uint32_t b = rendered[src_idx + 2];
                        uint32_t a = rendered[src_idx + 3];
                        pixels[dst_pixel_idx] = SDL_MapRGBA(surface->format, r, g, b, a);
                    }
                }
            }
        }
    }
    
    // Unlock and update
    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }
    
    SDL_UpdateWindowSurface(window);
}

void BrowserWindow::update_url_bar_from_input(const std::string& input) {
    current_url = input;
}

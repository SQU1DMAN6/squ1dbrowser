#pragma once

#include <string>
#include <vector>
#include <cstdint>

class RendererBridge {
public:
    RendererBridge();
    ~RendererBridge();
    
    // Send render request to Rust renderer
    bool request_render(const std::string& url, int width, int height);
    
    // Get rendered content
    std::vector<uint8_t> get_rendered_content() const;
    
    // Parse HTML and return rendered pixel data
    std::vector<uint8_t> render_html(const std::string& html, int width, int height);
    
private:
    // IPC communication setup
    bool setup_ipc();
};

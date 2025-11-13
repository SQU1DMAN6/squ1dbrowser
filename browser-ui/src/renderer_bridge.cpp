#include "renderer_bridge.h"
#include "bmp_loader.h"
#include <iostream>
#include <cstdlib>
#include <sstream>

RendererBridge::RendererBridge() {
    setup_ipc();
}

RendererBridge::~RendererBridge() {}

bool RendererBridge::setup_ipc() {
    std::cout << "IPC bridge initialized" << std::endl;
    return true;
}

bool RendererBridge::request_render(const std::string& url, int width, int height) {
    std::cout << "Render request: " << url << " (" << width << "x" << height << ")" << std::endl;
    
    // For now, render a simple test HTML that mentions the URL
    std::string html = "<html><body><h1>Loading: " + url + "</h1><p>Page content would appear here.</p></body></html>";
    
    // Call the Rust renderer process with absolute path
    std::stringstream cmd;
    cmd << "/home/qchef/Documents/squ1dbrowser/renderer/target/release/renderer \"" << html << "\" " 
        << width << " " << height << " /tmp/squ1d_render.bmp 2>/dev/null";
    
    int ret = system(cmd.str().c_str());
    if (ret != 0) {
        std::cerr << "Renderer invocation failed with code " << ret << std::endl;
        return false;
    }
    
    return true;
}

std::vector<uint8_t> RendererBridge::get_rendered_content() const {
    // Load the BMP that was just rendered
    BMPLoader::Image img = BMPLoader::load("/tmp/squ1d_render.bmp");
    
    if (img.pixels.empty()) {
        std::cerr << "Failed to load rendered content from BMP" << std::endl;
        return std::vector<uint8_t>();
    }
    
    return img.pixels;
}

std::vector<uint8_t> RendererBridge::render_html(const std::string& html, int width, int height) {
    // Invoke renderer
    if (!request_render(html, width, height)) {
        // Return white background on error
        std::vector<uint8_t> pixels(width * height * 4);
        for (size_t i = 0; i < pixels.size(); i += 4) {
            pixels[i + 0] = 255;     // R
            pixels[i + 1] = 255;     // G
            pixels[i + 2] = 255;     // B
            pixels[i + 3] = 255;     // A
        }
        return pixels;
    }
    
    return get_rendered_content();
}


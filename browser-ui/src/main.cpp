#include "browser_window.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Starting SQU1D Browser..." << std::endl;
    
    BrowserWindow browser(1200, 800, "SQU1D Browser - Falkon macOS Theme");
    
    if (!browser.is_running()) {
        std::cerr << "Failed to initialize browser window" << std::endl;
        return 1;
    }
    
    browser.run();
    
    std::cout << "Browser closed" << std::endl;
    return 0;
}

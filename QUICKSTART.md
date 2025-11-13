# SQU1D Browser - Quick Start Guide

## Overview

SQU1D Browser 2.0 is a complete rewrite featuring:
- **Rust Renderer**: High-performance HTML/CSS rendering engine
- **C++ UI**: Custom browser interface inspired by Falkon macOS theme
- **Multi-tab support**: Full tabbed browsing experience
- **Network support**: HTTP client for web requests

## Project Structure

```
squ1dbrowser/
├── renderer/              # Rust web rendering engine
│   ├── Cargo.toml        # Rust dependencies
│   └── src/
│       ├── main.rs       # Standalone renderer binary
│       ├── lib.rs        # Public API
│       ├── dom.rs        # Document Object Model
│       ├── html_parser.rs # HTML5 parser
│       ├── css_parser.rs  # CSS parser
│       ├── layout.rs      # Layout calculation engine
│       ├── renderer.rs    # Pixel rendering pipeline
│       └── http_client.rs # Network requests
│
├── browser-ui/           # C++ browser frontend
│   ├── CMakeLists.txt    # Build configuration
│   └── src/
│       ├── main.cpp           # Entry point
│       ├── browser_window.cpp  # Main window logic
│       ├── ui_renderer.cpp     # 2D rendering primitives
│       ├── tab_manager.cpp     # Tab management
│       └── renderer_bridge.cpp # IPC communication
│
└── shared/              # Shared resources (future)
```

## Installation

### Prerequisites

#### Linux (Ubuntu/Debian)
```bash
# Rust toolchain
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

# C++ build tools
sudo apt-get install -y build-essential cmake libsdl2-dev

# Windows (MSVC)
# Install Visual Studio 2022 with C++ workload
# Install Rust from https://rustup.rs/
```

### Build

```bash
# Build both components
make all

# Or build individually
make build-renderer  # Rust renderer only
make build-ui        # C++ UI only
```

## Usage

### Run the Browser
```bash
make run-browser
```

### Run the Renderer (standalone)
```bash
make run-renderer
```

### Development Mode
```bash
make dev  # Builds with debug symbols
make run-browser
```

## Architecture

### Renderer (Rust)

The renderer is a standalone engine that:

1. **Parses HTML** using a custom tokenizer
2. **Parses CSS** and applies styles to elements
3. **Calculates Layout** using the layout engine
4. **Renders to Pixels** producing RGBA output

```
HTML Input → Parser → DOM Tree → CSS Rules → Layout Tree → Pixel Buffer
```

### Browser UI (C++)

The browser UI provides:

1. **Window Management** via SDL2
2. **UI Rendering** with custom 2D primitives
3. **Tab Management** for multi-tab browsing
4. **Event Handling** for user interactions
5. **Renderer Bridge** for IPC with the Rust renderer

### Communication

The bridge uses message passing to communicate between:
- **Browser UI (C++)** - Sends render requests, displays results
- **Renderer (Rust)** - Receives URLs, returns pixel data

## UI Design

The browser features a **Falkon macOS-inspired** design with:

- Clean toolbar with navigation buttons (Back, Forward, Refresh)
- URL bar with rounded corners and subtle borders
- Tab bar with active/inactive states
- Smooth button hover effects
- Light gray color scheme (240-245 RGB base)
- Rounded corners on interactive elements (4-6px radius)

### Color Palette

| Element | Color | RGB |
|---------|-------|-----|
| Toolbar | Light Gray | (240, 240, 240) |
| Active Tab | White | (255, 255, 255) |
| Inactive Tab | Gray | (230, 230, 230) |
| Buttons | Light Gray | (220, 220, 220) |
| Text | Dark Gray | (0, 0, 0) |
| Borders | Medium Gray | (200, 200, 200) |

## Features

✅ **Multi-tab browsing** - Create, switch, and close tabs
✅ **Navigation** - Back, Forward, Refresh buttons
✅ **URL bar** - Type URLs and navigate
✅ **HTML rendering** - Basic HTML5 parsing and display
✅ **CSS support** - Style application and layout
✅ **Responsive** - Resizable window with responsive layout
✅ **Clean UI** - Falkon macOS-inspired interface

## Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| Escape | Unfocus URL bar |
| Enter | Navigate to URL (when URL bar focused) |
| Cmd+W | Close current tab |

## Limitations & TODO

- ⚠️ JavaScript execution not yet implemented
- ⚠️ Limited HTML5 support (basic tags only)
- ⚠️ CSS support is simplified
- ⚠️ No image rendering yet
- ⚠️ No form submission support
- ⚠️ Single-threaded renderer (blocking operations)

### Planned Enhancements

- [ ] Full HTML5 support with html5ever parser
- [ ] JavaScript engine (V8 or SpiderMonkey)
- [ ] WebGL support
- [ ] GPU-accelerated rendering
- [ ] Bookmarks and history persistence
- [ ] Developer tools
- [ ] Extension system
- [ ] Print functionality
- [ ] Find-in-page
- [ ] Settings dialog

## Performance

Initial performance targets:
- **Renderer**: < 16ms for 1024x768 page
- **UI**: 60 FPS rendering
- **Memory**: < 200MB base

## Troubleshooting

### Build Issues

**CMake not found**
```bash
sudo apt-get install cmake
```

**SDL2 not found**
```bash
sudo apt-get install libsdl2-dev
```

**Rust not found**
```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env
```

### Runtime Issues

**Window fails to open**
- Check SDL2 installation
- Verify graphics drivers are up to date

**Rendering not working**
- Ensure both renderer and UI built successfully
- Check for IPC communication issues

## Contributing

This is a personal project, but feel free to fork and customize!

## License

MIT License - See LICENSE file for details

## References

- [Falkon Browser](https://www.falkon.org/)
- [WebKit Architecture](https://webkit.org/)
- [Servo Browser Engine](https://servo.org/)
- [SDL2 Documentation](https://wiki.libsdl.org/)

# SQU1D Browser - Build Instructions

## Requirements

- CMake 3.20+
- SDL2 development libraries
- C++17 compatible compiler
- Rust toolchain (for the renderer engine)
- Cargo

## Building the Renderer

```bash
cd renderer
cargo build --release
```

## Building the Browser UI

### Install Dependencies (Ubuntu/Debian)

```bash
sudo apt-get install libsdl2-dev cmake build-essential
```

### Configure and Build

```bash
cd browser-ui
mkdir build
cd build
cmake ..
make
```

## Running

### Option 1: Run the browser (with integrated renderer)
```bash
./build/squ1d-browser
```

### Option 2: Run the renderer in isolation
```bash
cd renderer
cargo run --release
```

## Project Structure

```
.
├── renderer/              # Rust web rendering engine
│   ├── src/
│   │   ├── main.rs       # Renderer binary entry point
│   │   ├── lib.rs        # Library exports
│   │   ├── dom.rs        # Document Object Model
│   │   ├── html_parser.rs # HTML parsing
│   │   ├── css_parser.rs  # CSS parsing
│   │   ├── layout.rs      # Layout engine
│   │   ├── renderer.rs    # Rendering pipeline
│   │   └── http_client.rs # Network requests
│   └── Cargo.toml
│
├── browser-ui/           # C++ browser frontend
│   ├── src/
│   │   ├── main.cpp              # Entry point
│   │   ├── browser_window.h/cpp  # Main window
│   │   ├── ui_renderer.h/cpp     # UI rendering
│   │   ├── tab_manager.h/cpp     # Tab management
│   │   ├── renderer_bridge.h/cpp # IPC bridge
│   │   └── ui_types.h            # UI types and theme
│   └── CMakeLists.txt
│
└── README.md
```

## Architecture

### Renderer (Rust)
- **DOM Parser**: Parses HTML into a document tree
- **CSS Parser**: Extracts and applies styles
- **Layout Engine**: Calculates element positions and sizes
- **Renderer**: Converts layout to pixel data

### Browser UI (C++)
- **Window Manager**: SDL2-based windowing
- **UI Renderer**: Custom 2D drawing pipeline
- **Tab Manager**: Multi-tab support
- **Renderer Bridge**: IPC communication with Rust renderer

## Features

✅ Multi-tab browsing
✅ Back/Forward navigation
✅ Refresh button
✅ URL bar
✅ Falkon macOS-inspired UI
✅ Custom HTML/CSS rendering
✅ Responsive design

## Future Enhancements

- [ ] Full HTML5 support
- [ ] JavaScript engine integration
- [ ] GPU-accelerated rendering
- [ ] Bookmarks and history
- [ ] Extensions support
- [ ] Developer tools

# SQU1D Browser — Zero-Dependency Build

A custom web browser built **completely from scratch** with zero external dependencies for core rendering logic.

## Architecture

```
┌─────────────────────────────────────────┐
│     C++ Browser UI (SDL2 window)        │
│  ┌─────────────────────────────────┐   │
│  │ Toolbar + Tabs + URL Bar        │   │
│  ├─────────────────────────────────┤   │
│  │   Rendered Content Area         │   │
│  │  (BMP from Rust Renderer)       │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
                    ↑ IPC
                    │ (BMP via /tmp)
┌─────────────────────────────────────────┐
│   Rust Web Renderer                     │
│  • HTML Parser (custom)                 │
│  • CSS Parser (custom)                  │
│  • Layout Engine                        │
│  • Bitmap Font Text Rendering           │
│  • BMP Writer                           │
│  • HTTP/1.0 Client (plain TCP)          │
└─────────────────────────────────────────┘
```

## Zero Dependencies Achieved

### Rust Renderer (`renderer/`)
- **No HTML parsing libraries** – custom SimpleTokenizer handles DOCTYPE, comments, attributes
- **No CSS libraries** – built-in CssParser with inline style support
- **No image crates** – SimpleImage struct with native BMP writer
- **No async/TLS** – synchronous blocking HTTP/1.0 over raw TCP
- **Pure Rust dependencies only**: bytes, serde, serde_json (all pure Rust, no native bindings)

### C++ Browser UI (`browser-ui/`)
- **SDL2** – only external native dependency (lightweight window/event management)
- **No Skia** – custom framebuffer-based rendering with rectangles and lines
- **No font libraries** – 3x5 bitmap font data embedded in headers
- **No network libraries** – invokes Rust renderer via `system()` call

### Shared
- `bitmap_font.h` – minimal 3x5 glyph set (95 ASCII characters)

## Building

### Requirements
- **Rust** (stable) with cargo
- **C++ compiler** (GCC/Clang, C++17)
- **CMake** 3.x
- **SDL2 development libraries**

On Fedora:
```bash
sudo dnf install -y openssl-devel SDL2-devel pkgconfig
```

On Debian/Ubuntu:
```bash
sudo apt update
sudo apt install -y build-essential pkg-config libssl-dev libsdl2-dev
```

### Build Steps

```bash
# Build Rust renderer
cd renderer
cargo build --release

# Build C++ UI
cd ..
mkdir -p build && cd build
cmake ../browser-ui
make -j$(nproc)

# Run browser
cd ..
./build/squ1d-browser
```

Or use the Makefile:
```bash
make all           # Build everything
make run-browser   # Run the browser
make clean         # Clean build artifacts
```

## Component Breakdown

### Renderer (`renderer/`)

**HTML Parser** (`src/html_parser.rs`):
- Tokenizes HTML into tags, attributes, text nodes
- Handles `<!DOCTYPE>`, `<!-- comments -->`, nested elements
- Returns DOM tree

**Layout Engine** (`src/layout.rs`):
- Simple block-flow layout
- Box model with width/height estimation
- Recursive layout tree construction

**Renderer** (`src/renderer.rs`):
- Paints layout boxes as colored rectangles
- Uses bitmap font for text rendering
- Outputs RGBA framebuffer
- Writes BMP files (24-bit, BGR format)

**HTTP Client** (`src/http_client.rs`):
- Blocking TCP/IP socket-based implementation
- HTTP/1.0 GET requests only
- Parses status line and body
- No TLS/HTTPS support yet

**Bitmap Font** (`src/bitmap_font.rs`):
- 3x5 pixel glyphs for ASCII 32-126
- Embedded as byte arrays
- 475 bytes total data

### Browser UI (`browser-ui/`)

**UIRenderer** (`src/ui_renderer.cpp`):
- RGBA framebuffer-based drawing
- Primitives: rectangles, lines, rounded corners
- Text rendering with bitmap font

**BrowserWindow** (`src/browser_window.cpp`):
- SDL2 window and event loop (~60 FPS)
- Navigation history, tab management
- URL bar, toolbar buttons
- Mouse and keyboard event handling

**RendererBridge** (`src/renderer_bridge.cpp`):
- Spawns Rust renderer process
- Loads BMP output via BMPLoader
- Blends rendered content into window

**TabManager** (`src/tab_manager.cpp`):
- Tab creation/switching/closing
- URL and title tracking

## Features Implemented

✅ **Core Rendering**
- HTML parsing (custom, no external crates)
- CSS parsing (custom, no external crates)
- Layout calculation
- Text rendering with bitmap fonts
- Output to BMP format

✅ **Browser UI**
- Tabbed browsing interface
- Navigation toolbar (back, forward, reload)
- URL bar
- Tab bar with active indicator
- Window resizing support

✅ **Display**
- Real-time rendering integration
- Pixel blending between UI and content
- 60 FPS refresh

## Limitations & Future Work

⚠️ **Current Limitations:**
- HTTP/1.0 only (no HTTPS/TLS)
- Plain `http://` URLs only
- No JavaScript execution
- Minimal HTML/CSS support (no flexbox, grid, etc.)
- No cookies or sessions
- No caching
- Text rendering at 3x5 pixel resolution

🔮 **Future Enhancements:**
- HTTPS support (pure Rust TLS, e.g., rustls, or minimal handshake)
- JavaScript VM integration
- Advanced CSS layout (flexbox, grid)
- Better typography (scalable fonts)
- HTML5 semantic support
- Form handling
- Cookies and local storage
- Performance optimization (rendering caching)

## File Structure

```
squ1dbrowser/
├── renderer/                    # Rust web renderer
│   ├── Cargo.toml
│   └── src/
│       ├── main.rs             # CLI entry point
│       ├── lib.rs              # Library exports
│       ├── dom.rs              # Document tree types
│       ├── html_parser.rs       # Custom HTML tokenizer
│       ├── css_parser.rs        # CSS parsing
│       ├── layout.rs            # Layout engine
│       ├── renderer.rs          # Paint/render logic
│       ├── http_client.rs       # TCP HTTP/1.0 client
│       └── bitmap_font.rs       # Font data
├── browser-ui/                  # C++ browser UI
│   ├── CMakeLists.txt
│   └── src/
│       ├── main.cpp             # SDL2 main loop
│       ├── browser_window.h/.cpp # Window and events
│       ├── ui_renderer.h/.cpp    # Framebuffer drawing
│       ├── ui_types.h            # Color, Rect, theme
│       ├── tab_manager.h/.cpp    # Tab management
│       ├── renderer_bridge.h/.cpp # Renderer IPC
│       └── bmp_loader.h          # BMP file reader
├── shared/                       # Shared headers
│   └── bitmap_font.h             # Bitmap font data (C++)
├── Makefile                      # Top-level build orchestration
└── README.md                     # This file
```

## Dependency Tree (Rust)

```
squ1d-renderer
├── bytes (pure Rust)
├── serde (pure Rust with proc-macro derive)
└── serde_json (pure Rust)

No native bindings. No C/C++ libraries required.
No OpenSSL, no network crates, no image processing crates.
```

## Testing

### Render a test page

```bash
./renderer/target/release/renderer "<html><body><h1>Test</h1></body></html>" 800 600 /tmp/test.bmp
```

Outputs `/tmp/test.bmp` with rendered content.

### Run the browser

```bash
./build/squ1d-browser
```

Opens a window with the browser UI. Navigate to `http://example.com` (HTTP only).

## Performance Notes

- **Rendering**: ~100-500ms per page (Rust renderer, depends on HTML size)
- **UI loop**: 60 FPS (16ms/frame)
- **Memory**: ~10-50 MB (depending on page complexity)

## Known Issues

1. **Segmentation fault on window maximize** (SDL/GTK interaction, non-critical)
2. **HTTPS not supported** – only plain HTTP
3. **Large pages may be slow** – no optimization yet
4. **Limited DOM support** – basic elements only

## Contributing

To add features:
1. **New HTML tags** → update `dom.rs` and `html_parser.rs`
2. **New CSS properties** → update `css_parser.rs` and `layout.rs`
3. **Better fonts** → extend `bitmap_font.rs` with larger glyphs
4. **HTTPS support** → implement TLS in `http_client.rs` (consider rustls or minimal handshake)

## License

This project is a from-scratch educational implementation. Feel free to use and modify.

---

**Built from scratch with zero external web rendering dependencies.**

# SQU1D Browser Architecture

## High-Level Overview

```
┌─────────────────────────────────────────────────────────────┐
│                     Browser UI (C++)                         │
│  ┌──────────────────────────────────────────────────────┐  │
│  │           SDL2 Window & Event Loop                    │  │
│  ├──────────────────────────────────────────────────────┤  │
│  │  Navigation Buttons │ URL Bar │ Tab Bar │ + Button   │  │
│  ├──────────────────────────────────────────────────────┤  │
│  │              Rendered Page Display                    │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                           ↕ IPC Bridge
┌─────────────────────────────────────────────────────────────┐
│              Renderer Engine (Rust)                         │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  HTTP Client → HTML Parser → DOM Tree                │  │
│  │       ↓            ↓            ↓                     │  │
│  │  CSS Parser → Layout Engine → Pixel Renderer        │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

## Component Details

### Browser UI (C++)

#### Classes

**BrowserWindow**
- Main application window
- Event handling (mouse, keyboard, window events)
- Manages navigation state
- Coordinates between UI renderer and renderer bridge

**UIRenderer**
- Custom 2D drawing primitives
- Renders toolbar, tabs, buttons, URL bar
- Falkon macOS theme implementation
- Direct pixel buffer manipulation (RGBA)

**TabManager**
- Manages browser tabs
- Handles tab creation, switching, closing
- Maintains active tab state

**RendererBridge**
- Inter-process communication with Rust renderer
- Converts between C++ and Rust data formats
- Manages async render requests

#### UI Layout

```
[0, 0]
┌──────────────────────────────────────────────────────┐
│ ← → ⟳ │   URL Bar   │ +  [Tab 1] [Tab 2] [Tab 3]  │ ← 50px
├──────────────────────────────────────────────────────┤
│                                                      │
│              Rendered Content Area                   │
│           (from Rust Renderer)                       │
│                                                      │
│                                                      │
└──────────────────────────────────────────────────────┘
```

### Renderer Engine (Rust)

#### Modules

**html_parser.rs**
- Tokenizes HTML input
- Builds DOM tree from tokens
- Handles basic HTML5 tags
- Simplified error recovery

**css_parser.rs**
- Parses CSS rule blocks
- Extracts selectors and properties
- Calculates CSS specificity

**dom.rs**
- Document Object Model representation
- Element nodes with attributes
- Text nodes
- Style rules

**layout.rs**
- Layout box calculation
- Position and size computation
- Block and inline layout models (simplified)
- Bounding boxes for rendering

**renderer.rs**
- Converts layout tree to pixels
- Draws boxes, text, and content
- RGBA buffer output
- PNG export capability

**http_client.rs**
- Async HTTP requests using Hyper
- Fetches remote content
- Custom header support

#### Data Flow

```
Input URL
   ↓
HTTP Client (fetch content)
   ↓
HTML Parser (tokenize & build DOM)
   ↓
CSS Parser (extract styles)
   ↓
Layout Engine (calculate positions)
   ↓
Renderer (paint to pixels)
   ↓
Output: RGBA Buffer [width × height × 4 bytes]
```

## Communication Protocol

### IPC Between C++ and Rust

**Render Request**
```
C++ → Rust:
{
  "type": "render",
  "url": "https://example.com",
  "width": 1024,
  "height": 768
}
```

**Render Response**
```
Rust → C++:
{
  "type": "render_result",
  "pixels": [u8; width × height × 4],
  "width": 1024,
  "height": 768
}
```

Currently implemented as:
- Process spawn with message passing
- Files as temporary storage
- Extensible for socket/pipe-based IPC

## Threading Model

### Current (Single-threaded)
- Main thread handles UI events and rendering
- Blocking calls to renderer

### Planned (Multi-threaded)
```
┌─────────────────────┐
│   Main UI Thread    │ ← Handles events, draws UI
└────────┬────────────┘
         │
         ├─→ Event Loop Thread
         │
         └─→ Renderer Thread ← Calls Rust renderer
```

## Memory Management

### Frame Buffer
- Allocated per window resize
- RGBA format: 4 bytes per pixel
- Example: 1200 × 800 × 4 = 3.84 MB

### DOM Tree
- Cloned for each render
- Reference counting for elements
- Future: Implement incremental rendering

### Layout Tree
- Computed on each render
- Cached between renders
- Invalidated on size changes

## Performance Characteristics

### Current
- HTML parsing: O(n) where n = input size
- Layout calculation: O(n) where n = element count
- Rendering: O(w × h) where w, h = dimensions

### Bottlenecks
- Single-threaded execution
- Full DOM rebuild on each render
- Pixel-by-pixel text rendering

### Optimization Opportunities
- Incremental layout with dirty tracking
- GPU acceleration (Skia when available)
- Text rendering library integration
- Caching rendered glyphs

## Extension Points

### Adding a New HTML Tag

1. Update parser in `html_parser.rs`
2. Handle in layout engine (`layout.rs`)
3. Add rendering logic (`renderer.rs`)

### Adding CSS Properties

1. Parse in `css_parser.rs`
2. Store in element styles
3. Apply in layout phase
4. Implement rendering

### Custom Rendering

Modify `renderer.rs` paint methods:
- `paint_tree()` - Main rendering dispatch
- `draw_text()` - Text rendering
- Theme colors in `ui_types.h`

## Build Considerations

### Cargo Dependencies (Renderer)
- `tokio` - Async runtime
- `hyper` - HTTP client
- `image` - Image processing
- `png` - PNG encoding
- `cssparser` - CSS parsing library
- `html5ever` - Future HTML5 parsing

### CMake Dependencies (UI)
- `SDL2` - Window management
- `pkg-config` - Dependency detection
- Optional: Skia for advanced rendering

## Future Architecture

### Phase 2: Advanced Rendering
```
┌─────────────────┐
│  WebRender      │ ← GPU-accelerated rendering
└────────┬────────┘
         │
    ┌────┴────────────────┐
    │                     │
┌───▼────┐        ┌──────▼──┐
│ Layout │        │ Paint   │
│ Tree   │        │ Commands│
└────────┘        └─────────┘
```

### Phase 3: JavaScript Engine
```
┌─────────────────────────┐
│   SpiderMonkey / V8     │
├─────────────────────────┤
│   DOM API Bindings      │
├─────────────────────────┤
│   Event System          │
└─────────────────────────┘
```

### Phase 4: Full WebKit Compatibility
- Service Workers
- Progressive Web Apps
- WebAssembly support
- Full CSS Grid/Flexbox
- SVG rendering

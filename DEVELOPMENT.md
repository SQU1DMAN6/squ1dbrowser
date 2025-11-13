# SQU1D Browser - Developer Setup Guide

## Prerequisites Installation

### Linux (Ubuntu/Debian)

```bash
# Update package lists
sudo apt-get update

# Install Rust
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env

# Install build tools
sudo apt-get install -y \
    build-essential \
    cmake \
    libsdl2-dev \
    pkg-config

# Verify installations
rustc --version          # Should be 1.70+
cargo --version
g++ --version           # Should be 11+
cmake --version         # Should be 3.20+
sdl2-config --version
```

### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Rust
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env

# Install dependencies via Homebrew
brew install cmake sdl2

# Verify installations
rustc --version
cmake --version
```

### Windows (MSVC)

1. Install Visual Studio 2022 with C++ workload
2. Install CMake from https://cmake.org/download/
3. Install Rust from https://rustup.rs/
4. Install SDL2 development libraries:
   ```bash
   # Using vcpkg
   vcpkg install sdl2:x64-windows
   ```

## Project Setup

```bash
# Clone the repository
git clone https://github.com/SQU1DMAN6/squ1dbrowser.git
cd squ1dbrowser

# Initialize git hooks (optional)
# git config core.hooksPath .githooks

# View available commands
make help
```

## Building

### Development Build

```bash
# Build with debug symbols
make dev

# Individual components
make dev-renderer  # Rust renderer debug build
make dev-ui        # C++ UI debug build
```

### Release Build

```bash
# Optimized builds
make all

# Individual components
make build-renderer  # Rust renderer release
make build-ui        # C++ UI release
```

### Custom Build

```bash
# Renderer only
cd renderer
cargo build --release

# Browser UI only
cd browser-ui
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Running

### Development

```bash
# Run with live updates
make run-browser

# Or manually
./build/squ1d-browser

# With debug output
RUST_BACKTRACE=1 ./build/squ1d-browser
```

### Testing Renderer

```bash
# Test HTML parsing
cd renderer
cargo test

# Run renderer standalone
cargo run --release

# Build benchmarks
cargo bench
```

## Development Workflow

### Making Changes to Renderer

```bash
# Edit Rust code
vim renderer/src/html_parser.rs

# Rebuild
make build-renderer

# Run tests
cd renderer && cargo test

# Run full browser
make run-browser
```

### Making Changes to UI

```bash
# Edit C++ code
vim browser-ui/src/ui_renderer.cpp

# Rebuild
make build-ui

# Run browser
make run-browser
```

## Debugging

### Using GDB (Linux)

```bash
# Build with debug symbols
make dev-ui

# Run under GDB
gdb ./build/squ1d-browser

# Common commands:
# (gdb) run
# (gdb) break main
# (gdb) print variable
# (gdb) backtrace
# (gdb) quit
```

### Using LLDB (macOS)

```bash
# Build with debug symbols
make dev-ui

# Run under LLDB
lldb ./build/squ1d-browser

# Common commands:
# (lldb) run
# (lldb) breakpoint set --name main
# (lldb) frame variable
# (lldb) bt
# (lldb) quit
```

### Using Rust Backtrace

```bash
# Enable backtrace for Rust panics
RUST_BACKTRACE=1 cargo run

# Full backtrace
RUST_BACKTRACE=full cargo run
```

### Logging

Add logging to your code:

**Rust**:
```rust
eprintln!("Debug message: {}", value);
println!("Info message");
```

**C++**:
```cpp
std::cerr << "Debug message: " << value << std::endl;
std::cout << "Info message" << std::endl;
```

## Code Organization

### Renderer Code Structure

```
renderer/src/
├── lib.rs              # Public API
├── main.rs             # Binary entry point
├── dom.rs              # Document Object Model
├── html_parser.rs      # HTML parsing logic
├── css_parser.rs       # CSS parsing logic
├── layout.rs           # Layout calculations
├── renderer.rs         # Pixel rendering
└── http_client.rs      # HTTP requests
```

### UI Code Structure

```
browser-ui/src/
├── main.cpp                 # Entry point
├── browser_window.h/.cpp    # Main window implementation
├── ui_renderer.h/.cpp       # Rendering primitives
├── ui_types.h               # Types and theme colors
├── tab_manager.h/.cpp       # Tab management
└── renderer_bridge.h/.cpp   # IPC communication
```

## Adding Features

### Adding a New HTML Tag

1. **Parser** (`renderer/src/html_parser.rs`):
   ```rust
   // Add tag to recognized elements
   match tag_name.as_str() {
       "newtag" => {
           // Parse new tag
       }
       _ => {}
   }
   ```

2. **Layout** (`renderer/src/layout.rs`):
   ```rust
   fn estimate_height(element: &Element) -> f32 {
       match element.tag.as_str() {
           "newtag" => 40.0,
           _ => 20.0
       }
   }
   ```

3. **Renderer** (`renderer/src/renderer.rs`):
   ```rust
   "newtag" => {
       // Draw logic for new tag
   }
   ```

### Adding a UI Button

1. **Header** (`browser-ui/src/ui_types.h`):
   ```cpp
   struct Rect new_button_rect(10, 50, 40, 30);
   ```

2. **Implementation** (`browser-ui/src/ui_renderer.cpp`):
   ```cpp
   void UIRenderer::draw_new_button(const Rect& rect) {
       fill_rect(rect, Theme::BUTTON_BG);
       draw_rounded_rect(rect, Theme::SEPARATOR, 4.0f, 1.0f);
   }
   ```

3. **Event Handling** (`browser-ui/src/browser_window.cpp`):
   ```cpp
   if (new_button_rect.contains_point({x, y})) {
       // Handle button click
   }
   ```

## Performance Profiling

### Rust Profiling

```bash
# Build with optimization
cd renderer
RUSTFLAGS="-g" cargo build --release

# Analyze with perf (Linux)
perf record cargo run --release
perf report
```

### C++ Profiling

```bash
# Build with debug symbols
cd browser-ui
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Profile with gprof
gprof ./squ1d-browser gmon.out
```

## Documentation Standards

### Rust Documentation

```rust
/// Parses HTML content into a document tree.
///
/// # Arguments
/// * `html` - Raw HTML string
///
/// # Returns
/// A Result containing the parsed Document or an error message
///
/// # Example
/// ```
/// let html = "<html><body>Hello</body></html>";
/// let doc = HtmlParser::parse(html)?;
/// ```
pub fn parse(html: &str) -> Result<Document, String> {
    // Implementation
}
```

### C++ Documentation

```cpp
/// Renders a button at the specified rectangle
/// 
/// @param rect The button's position and size
/// @param label The button's text label
/// @param hovered Whether the button is in hover state
void draw_button(const Rect& rect, const std::string& label, bool hovered = false);
```

## Testing

### Running Tests

```bash
# Renderer tests
cd renderer
cargo test

# Specific test
cargo test html_parser

# With output
cargo test -- --nocapture
```

### Writing Tests

**Rust**:
```rust
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_simple_html() {
        let html = "<p>Test</p>";
        let doc = HtmlParser::parse(html);
        assert!(doc.is_ok());
    }
}
```

**C++**:
```cpp
#include <cassert>

void test_color_creation() {
    Color c = Color::from_hex(0xFF0000);
    assert(c.r == 255);
    assert(c.g == 0);
    assert(c.b == 0);
}
```

## Useful Tools

### Visual Studio Code Extensions

```
C/C++ (Microsoft)
rust-analyzer
Makefile Tools
CMake Tools
```

### Command Line Tools

```bash
# Code formatting (Rust)
cargo fmt

# Linting (Rust)
cargo clippy

# CMake formatting
sudo apt-get install cmake-format

# Size analysis
cargo bloat --release
```

## Troubleshooting

### Compilation Issues

**Undefined reference error**
- Check CMakeLists.txt links all necessary libraries
- Verify SDL2 installation: `pkg-config --cflags --libs sdl2`

**Rust compilation error**
- Update Rust: `rustup update`
- Clean and rebuild: `cd renderer && cargo clean && cargo build`

### Runtime Issues

**Segmentation fault**
- Rebuild with debug symbols: `make dev-ui`
- Run with debugger: `gdb ./build/squ1d-browser`

**SDL window not opening**
- Check X11 or Wayland setup on Linux
- Verify graphics drivers: `glxinfo | head`

## IDE Setup

### Visual Studio Code

Create `.vscode/settings.json`:
```json
{
    "rust-analyzer.checkOnSave.command": "clippy",
    "C_Cpp.default.compilerPath": "/usr/bin/g++",
    "CMake.configureArgs": ["-DCMAKE_BUILD_TYPE=Debug"],
    "[rust]": {
        "editor.formatOnSave": true,
        "editor.defaultFormatter": "rust-lang.rust-analyzer"
    }
}
```

### Clion (JetBrains)

1. Open project root
2. CLion detects CMakeLists.txt automatically
3. Go to File → Project Structure → Project
4. Select CMake as build tool
5. Configure Rust support: Languages & Frameworks → Rust

## Performance Tips

- Use `--release` builds for testing performance
- Profile before optimizing
- Consider incremental rendering for large pages
- Cache layout when possible
- Minimize allocations in hot paths

## Resources

- [Rust Book](https://doc.rust-lang.org/book/)
- [SDL2 Documentation](https://wiki.libsdl.org/)
- [CMake Manual](https://cmake.org/documentation/)
- [WebKit Architecture](https://webkit.org/architecture/)
- [Servo Engine](https://servo.org/)

---

Happy hacking! 🚀

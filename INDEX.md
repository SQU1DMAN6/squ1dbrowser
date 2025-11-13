# 📖 Documentation Index

Welcome to SQU1D Browser 2.0! Start here to understand the project.

## Quick Navigation

### 🚀 Getting Started
1. **[QUICKSTART.md](QUICKSTART.md)** - Start here! Setup and build instructions
2. **[PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)** - High-level overview of everything

### 📚 Learning & Understanding
3. **[ARCHITECTURE.md](ARCHITECTURE.md)** - Deep technical dive
4. **[DIAGRAMS.md](DIAGRAMS.md)** - Visual system diagrams
5. **[FEATURES.md](FEATURES.md)** - Feature list and roadmap

### 👨‍💻 Development
6. **[DEVELOPMENT.md](DEVELOPMENT.md)** - Developer setup and workflow
7. **[CHANGELOG.md](CHANGELOG.md)** - Version history

### 📖 Additional Resources
8. **[README.md](README.md)** - Main project documentation
9. This file - Documentation index

---

## For Different Audiences

### 👤 First-Time Users
1. Read [QUICKSTART.md](QUICKSTART.md) - Installation and first run
2. Glance at [FEATURES.md](FEATURES.md) - What you can do
3. Explore the UI - Try out features

### 👨‍💻 Developers
1. Read [DEVELOPMENT.md](DEVELOPMENT.md) - Setup your environment
2. Study [ARCHITECTURE.md](ARCHITECTURE.md) - Understand the system
3. Review [DIAGRAMS.md](DIAGRAMS.md) - See component relationships
4. Read source code - Start with `renderer/src/main.rs` or `browser-ui/src/main.cpp`

### 🏗️ Architects/Integrators
1. Read [ARCHITECTURE.md](ARCHITECTURE.md) - System design
2. Review [DIAGRAMS.md](DIAGRAMS.md) - Component interactions
3. Study IPC layer in [ARCHITECTURE.md](ARCHITECTURE.md) - Integration points
4. Review `browser-ui/src/renderer_bridge.*` - See communication pattern

### 🐛 Troubleshooters
1. Check [DEVELOPMENT.md](DEVELOPMENT.md) - Troubleshooting section
2. Review [CHANGELOG.md](CHANGELOG.md) - Known issues
3. Check [QUICKSTART.md](QUICKSTART.md) - Common setup issues

---

## Documentation Files at a Glance

| File | Purpose | Read Time |
|------|---------|-----------|
| **QUICKSTART.md** | Installation & first run | 5 min |
| **PROJECT_SUMMARY.md** | Complete overview | 10 min |
| **ARCHITECTURE.md** | Technical design | 20 min |
| **DIAGRAMS.md** | Visual explanations | 10 min |
| **FEATURES.md** | Feature list & roadmap | 10 min |
| **DEVELOPMENT.md** | Dev environment & workflow | 25 min |
| **CHANGELOG.md** | Version history | 5 min |
| **README.md** | Main documentation | 10 min |

**Total reading time: ~95 minutes** for complete understanding

---

## Quick Command Reference

```bash
# Setup
cd squ1dbrowser
make all                # Build everything

# Running
make run-browser        # Start the browser
make run-renderer       # Test renderer only

# Development
make dev                # Debug build
make clean              # Remove build artifacts
make help               # Show all commands
```

---

## Project Structure

```
📦 squ1dbrowser/
├── 📄 QUICKSTART.md          ← START HERE
├── 📄 PROJECT_SUMMARY.md     ← OVERVIEW
├── 📄 ARCHITECTURE.md        ← TECHNICAL DETAILS
├── 📄 DIAGRAMS.md            ← VISUAL GUIDES
├── 📄 FEATURES.md            ← WHAT'S INCLUDED
├── 📄 DEVELOPMENT.md         ← DEV SETUP
├── 📄 CHANGELOG.md           ← VERSION HISTORY
├── 📄 README.md              ← MAIN DOCS
│
├── 📁 renderer/              # Rust rendering engine
│   ├── src/
│   │   ├── main.rs           # Standalone binary
│   │   ├── lib.rs            # Public API
│   │   ├── dom.rs            # Document model
│   │   ├── html_parser.rs    # HTML parsing
│   │   ├── css_parser.rs     # CSS parsing
│   │   ├── layout.rs         # Layout engine
│   │   ├── renderer.rs       # Pixel rendering
│   │   └── http_client.rs    # Network requests
│   └── Cargo.toml            # Rust dependencies
│
├── 📁 browser-ui/            # C++ browser UI
│   ├── src/
│   │   ├── main.cpp          # Entry point
│   │   ├── browser_window.*  # Main window
│   │   ├── ui_renderer.*     # UI drawing
│   │   ├── tab_manager.*     # Tab handling
│   │   ├── renderer_bridge.* # IPC bridge
│   │   └── ui_types.h        # Types & theme
│   └── CMakeLists.txt        # Build config
│
└── 📁 shared/                # Shared resources
```

---

## Key Concepts

### Architecture
- **Two-process design**: Renderer (Rust) + UI (C++)
- **IPC communication**: Message passing between processes
- **Modular layout**: Separate concerns, easy to maintain
- **Extensible**: Ready for plugins and features

### Technology Stack
- **Renderer**: Rust + Tokio + Hyper
- **UI**: C++ + SDL2
- **Build**: Make + CMake + Cargo

### Key Features
✅ Multi-tab browsing
✅ HTML/CSS rendering
✅ Navigation controls
✅ Falkon macOS design
✅ Cross-platform

---

## Learning Paths

### Path 1: User Learning (30 min)
1. QUICKSTART.md (5 min)
2. Try the browser (10 min)
3. FEATURES.md (10 min)
4. Experiment (5 min)

### Path 2: Developer Learning (90 min)
1. QUICKSTART.md (5 min)
2. PROJECT_SUMMARY.md (10 min)
3. ARCHITECTURE.md (20 min)
4. DIAGRAMS.md (10 min)
5. DEVELOPMENT.md (25 min)
6. Review source code (20 min)

### Path 3: Architecture Review (45 min)
1. ARCHITECTURE.md (20 min)
2. DIAGRAMS.md (10 min)
3. Review `renderer_bridge.*` (10 min)
4. Review high-level code structure (5 min)

### Path 4: Customization (60 min)
1. QUICKSTART.md (5 min)
2. ARCHITECTURE.md (15 min)
3. DEVELOPMENT.md - Customization section (15 min)
4. Find relevant code files (10 min)
5. Implement changes (15 min)

---

## Common Questions

### Q: Where do I start?
**A:** Read [QUICKSTART.md](QUICKSTART.md) first - it has everything you need to get started.

### Q: How do I understand the architecture?
**A:** Read [ARCHITECTURE.md](ARCHITECTURE.md) for technical details and [DIAGRAMS.md](DIAGRAMS.md) for visual explanations.

### Q: How do I set up development?
**A:** Follow [DEVELOPMENT.md](DEVELOPMENT.md) for complete development environment setup.

### Q: What features are planned?
**A:** Check [FEATURES.md](FEATURES.md) for the roadmap and planned enhancements.

### Q: How do I troubleshoot?
**A:** See the troubleshooting section in [DEVELOPMENT.md](DEVELOPMENT.md).

### Q: What changed from version 1.0?
**A:** Read [CHANGELOG.md](CHANGELOG.md) for complete migration guide.

### Q: Can I customize the browser?
**A:** Yes! See customization section in [DEVELOPMENT.md](DEVELOPMENT.md).

---

## External Resources

### Official Documentation
- [Rust Book](https://doc.rust-lang.org/book/)
- [SDL2 Wiki](https://wiki.libsdl.org/)
- [CMake Manual](https://cmake.org/documentation/)

### Inspiration & References
- [Falkon Browser](https://www.falkon.org/)
- [Servo Engine](https://servo.org/)
- [WebKit Architecture](https://webkit.org/)

### Learning Resources
- [Web Browser Engineering](https://browser.engineering/)
- [Mozilla Developer Network](https://developer.mozilla.org/)

---

## Getting Help

1. **For setup issues**: See [QUICKSTART.md](QUICKSTART.md) troubleshooting
2. **For development help**: See [DEVELOPMENT.md](DEVELOPMENT.md) debugging section
3. **For architecture questions**: See [ARCHITECTURE.md](ARCHITECTURE.md)
4. **For feature requests**: See [FEATURES.md](FEATURES.md) roadmap
5. **For version migration**: See [CHANGELOG.md](CHANGELOG.md)

---

## Next Steps

### Recommended Order
1. ✅ Read this file (Documentation Index)
2. 📖 Read [QUICKSTART.md](QUICKSTART.md)
3. 🏗️ Build the project: `make all`
4. 🚀 Run the browser: `make run-browser`
5. 📚 Study [ARCHITECTURE.md](ARCHITECTURE.md)
6. 💻 Follow [DEVELOPMENT.md](DEVELOPMENT.md) for dev setup
7. 🔍 Explore source code

---

## Project Statistics

- **Total lines of code**: ~1,300
- **Documentation lines**: ~2,500
- **Build time**: ~2-3 minutes (full)
- **Binary size**: ~3-5 MB
- **Memory usage**: ~50-100 MB

---

## License

MIT License - See LICENSE file

---

## Last Updated

**2025-11-13**

---

**Happy browsing! 🚀**

For any questions, start with [QUICKSTART.md](QUICKSTART.md).

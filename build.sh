#!/usr/bin/env bash
# SQU1D Browser Build Helper Script

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

check_deps() {
    print_header "Checking Dependencies"
    
    # Check Rust
    if command -v cargo &> /dev/null; then
        print_success "Rust/Cargo found"
    else
        print_warning "Rust not found. Install from https://rustup.rs/"
        exit 1
    fi
    
    # Check C++ compiler
    if command -v g++ &> /dev/null || command -v clang++ &> /dev/null; then
        print_success "C++ compiler found"
    else
        print_warning "C++ compiler not found"
        exit 1
    fi
    
    # Check CMake
    if command -v cmake &> /dev/null; then
        print_success "CMake found"
    else
        print_warning "CMake not found. Install with: sudo apt-get install cmake"
        exit 1
    fi
    
    # Check SDL2
    if pkg-config --exists sdl2; then
        print_success "SDL2 found"
    else
        print_warning "SDL2 not found. Install with: sudo apt-get install libsdl2-dev"
        exit 1
    fi
}

build_renderer() {
    print_header "Building Rust Renderer"
    cd renderer
    cargo build --release
    print_success "Renderer built"
    cd ..
}

build_ui() {
    print_header "Building C++ Browser UI"
    mkdir -p build
    cd build
    cmake ../browser-ui
    make
    print_success "Browser UI built"
    cd ..
}

build_all() {
    check_deps
    build_renderer
    build_ui
    print_header "Build Complete!"
    echo -e "${GREEN}Run '${BLUE}./build/squ1d-browser${GREEN}' to start the browser${NC}"
}

case "${1:-all}" in
    all)
        build_all
        ;;
    renderer)
        build_renderer
        ;;
    ui)
        build_ui
        ;;
    check)
        check_deps
        ;;
    *)
        echo "Usage: $0 {all|renderer|ui|check}"
        exit 1
        ;;
esac

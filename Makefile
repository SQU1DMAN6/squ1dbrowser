.PHONY: all renderer browser-ui clean help build-renderer build-ui run-renderer run-browser dev

RENDERER_DIR := renderer
BROWSER_DIR := browser-ui
BUILD_DIR := build

all: build-renderer build-ui

help:
	@echo "SQU1D Browser - Build Commands"
	@echo "================================"
	@echo "make build-renderer    - Build Rust renderer"
	@echo "make build-ui          - Build C++ browser UI"
	@echo "make all               - Build both components"
	@echo "make run-renderer      - Run renderer in isolation"
	@echo "make run-browser       - Run browser UI"
	@echo "make dev               - Development build with debug symbols"
	@echo "make clean             - Clean all build artifacts"

build-renderer:
	@echo "Building Rust renderer..."
	cd $(RENDERER_DIR) && cargo build --release
	@echo "✓ Renderer built successfully"

dev-renderer:
	@echo "Building Rust renderer (debug)..."
	cd $(RENDERER_DIR) && cargo build

build-ui:
	@echo "Building C++ browser UI..."
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ../$(BROWSER_DIR) && make
	@echo "✓ Browser UI built successfully"

dev-ui:
	@echo "Building C++ browser UI (debug)..."
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug ../$(BROWSER_DIR) && make

run-renderer: build-renderer
	@echo "Running renderer..."
	cd $(RENDERER_DIR) && cargo run --release

run-browser: build-ui
	@echo "Running browser..."
	./$(BUILD_DIR)/squ1d-browser

dev: dev-renderer dev-ui
	@echo "✓ Development build complete"
	@echo "Run 'make run-browser' to start the browser"

clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	cd $(RENDERER_DIR) && cargo clean
	@echo "✓ Clean complete"

.DEFAULT_GOAL := help
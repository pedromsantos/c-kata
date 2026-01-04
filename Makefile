CMAKE = cmake
BUILD_DIR = build

.PHONY: all build test clean help

# Default target
all: build

# Configure and build with CMake
build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) ..
	@$(CMAKE) --build $(BUILD_DIR)

# Run all tests
test: build
	@cd $(BUILD_DIR) && ctest --output-on-failure

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR)
	@find src -name "*.o" -delete 2>/dev/null || true
	@find src -name "test_kata" -type f -delete 2>/dev/null || true
	@find src -name "*.dSYM" -type d -exec rm -rf {} + 2>/dev/null || true
	@echo "Clean complete."

# Show available targets
help:
	@echo "Available targets:"
	@echo "  make build  - Configure and build all katas with CMake"
	@echo "  make test   - Build and run all tests"
	@echo "  make clean  - Remove all build artifacts"
	@echo "  make help   - Show this help message"

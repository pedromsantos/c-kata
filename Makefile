CMAKE = cmake
BUILD_DIR = build

.PHONY: all build test clean help check-cmake

# Default target
all: build

# Check if cmake is installed
check-cmake:
	@if ! command -v $(CMAKE) >/dev/null 2>&1; then \
		echo "cmake not found. Installing cmake..."; \
		if command -v brew >/dev/null 2>&1; then \
			brew install cmake; \
		elif command -v apt-get >/dev/null 2>&1; then \
			sudo apt-get update && sudo apt-get install -y cmake; \
		elif command -v yum >/dev/null 2>&1; then \
			sudo yum install -y cmake; \
		else \
			echo "Error: Could not install cmake. Please install it manually."; \
			exit 1; \
		fi; \
	else \
		echo "cmake is already installed."; \
	fi

# Configure and build with CMake
build: check-cmake
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

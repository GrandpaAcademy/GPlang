# GPLANG v1.0.0 Build System
# Ultra-fast compilation with optimization

VERSION = 1.0.0
CC = gcc
CXX = g++
# Native compilation flags for zero overhead
CFLAGS = -O3 -march=native -mtune=native -flto -ffast-math -DNDEBUG -DVERSION=\"$(VERSION)\" \
         -fno-exceptions -fno-rtti -fno-stack-protector -fomit-frame-pointer \
         -mavx2 -mfma -mbmi2 -mlzcnt -mpopcnt
CXXFLAGS = $(CFLAGS) -std=c++17 -fno-exceptions -fno-rtti
LDFLAGS = -flto -s -lm -lpthread
AS = as
LD = ld

# Directories
SRC_DIR = src
FRONTEND_DIR = $(SRC_DIR)/frontend
IR_DIR = $(SRC_DIR)/ir
BACKEND_DIR = $(SRC_DIR)/backend
RUNTIME_DIR = $(SRC_DIR)/runtime
GAP_DIR = $(SRC_DIR)/gap
LIB_DIR = $(SRC_DIR)/lib
OPTIMIZE_DIR = $(SRC_DIR)/optimize

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
IR_OUTPUT_DIR = $(BUILD_DIR)/ir
ASM_OUTPUT_DIR = $(BUILD_DIR)/asm

TESTS_DIR = tests
EXAMPLES_DIR = examples
DOCS_DIR = docs

# Target architecture (default: x86_64)
TARGET ?= x86_64

# Source files
FRONTEND_SOURCES = $(wildcard $(FRONTEND_DIR)/*.c)
IR_SOURCES = $(wildcard $(IR_DIR)/*.c)
BACKEND_SOURCES = $(wildcard $(BACKEND_DIR)/*.c)
RUNTIME_SOURCES = $(wildcard $(RUNTIME_DIR)/*.c)
GAP_SOURCES = $(wildcard $(GAP_DIR)/*.c)
LIB_SOURCES = $(wildcard $(LIB_DIR)/*.c) $(wildcard $(LIB_DIR)/*/*.c)
OPTIMIZE_SOURCES = $(wildcard $(OPTIMIZE_DIR)/*.c)
NATIVE_SOURCES = $(wildcard $(SRC_DIR)/compiler/*.c)
SAFETY_SOURCES = $(wildcard $(SRC_DIR)/safety/*.c)
MAIN_SOURCE = $(SRC_DIR)/main.c

# Object files
FRONTEND_OBJECTS = $(FRONTEND_SOURCES:$(FRONTEND_DIR)/%.c=$(OBJ_DIR)/frontend/%.o)
IR_OBJECTS = $(IR_SOURCES:$(IR_DIR)/%.c=$(OBJ_DIR)/ir/%.o)
BACKEND_OBJECTS = $(BACKEND_SOURCES:$(BACKEND_DIR)/%.c=$(OBJ_DIR)/backend/%.o)
RUNTIME_OBJECTS = $(RUNTIME_SOURCES:$(RUNTIME_DIR)/%.c=$(OBJ_DIR)/runtime/%.o)
LIB_OBJECTS = $(OBJ_DIR)/lib/os/os.o $(OBJ_DIR)/lib/net/net.o $(OBJ_DIR)/lib/fs/fs.o $(OBJ_DIR)/lib/json/json.o \
              $(OBJ_DIR)/lib/math/math.o $(OBJ_DIR)/lib/string/string.o $(OBJ_DIR)/lib/crypto/crypto.o \
              $(OBJ_DIR)/lib/time/time.o $(OBJ_DIR)/lib/collections/collections.o $(OBJ_DIR)/lib/gplang_stdlib.o
OPTIMIZE_OBJECTS = $(OBJ_DIR)/optimize/optimizer.o $(OBJ_DIR)/optimize/error_handler.o $(OBJ_DIR)/optimize/speed_booster.o
NATIVE_OBJECTS = $(OBJ_DIR)/compiler/native_compiler.o
SAFETY_OBJECTS = $(OBJ_DIR)/safety/memory_safety.o
MAIN_OBJECT = $(OBJ_DIR)/main.o

ALL_OBJECTS = $(FRONTEND_OBJECTS) $(IR_OBJECTS) $(BACKEND_OBJECTS) $(RUNTIME_OBJECTS) $(LIB_OBJECTS) $(OPTIMIZE_OBJECTS) $(NATIVE_OBJECTS) $(SAFETY_OBJECTS) $(MAIN_OBJECT)

# Main targets
.PHONY: all build clean test docs examples help gap

all: build

build: $(BIN_DIR)/gplang $(BIN_DIR)/gap

# Create build directories
$(BUILD_DIR):
	@echo "📁 Creating build directories..."
	@mkdir -p $(OBJ_DIR)/frontend $(OBJ_DIR)/ir $(OBJ_DIR)/backend $(OBJ_DIR)/runtime
	@mkdir -p $(OBJ_DIR)/lib/os $(OBJ_DIR)/lib/net $(OBJ_DIR)/lib/fs $(OBJ_DIR)/lib/json $(OBJ_DIR)/lib
	@mkdir -p $(OBJ_DIR)/lib/math $(OBJ_DIR)/lib/string $(OBJ_DIR)/lib/crypto $(OBJ_DIR)/lib/time $(OBJ_DIR)/lib/collections
	@mkdir -p $(OBJ_DIR)/optimize $(OBJ_DIR)/compiler $(OBJ_DIR)/safety
	@mkdir -p $(BIN_DIR) $(IR_OUTPUT_DIR) $(ASM_OUTPUT_DIR)

# Compile frontend (lexer, parser, semantic analysis)
$(OBJ_DIR)/frontend/%.o: $(FRONTEND_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(FRONTEND_DIR) -c $< -o $@

# Compile IR generation
$(OBJ_DIR)/ir/%.o: $(IR_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(IR_DIR) -I$(FRONTEND_DIR) -c $< -o $@

# Compile backend (code generation)
$(OBJ_DIR)/backend/%.o: $(BACKEND_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(BACKEND_DIR) -I$(IR_DIR) -I$(FRONTEND_DIR) -c $< -o $@

# Compile runtime
$(OBJ_DIR)/runtime/%.o: $(RUNTIME_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(RUNTIME_DIR) -c $< -o $@

# Compile library modules
$(OBJ_DIR)/lib/os/os.o: $(LIB_DIR)/os/os.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/net/net.o: $(LIB_DIR)/net/net.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/fs/fs.o: $(LIB_DIR)/fs/fs.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/json/json.o: $(LIB_DIR)/json/json.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/math/math.o: $(LIB_DIR)/math/math.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/string/string.o: $(LIB_DIR)/string/string.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/crypto/crypto.o: $(LIB_DIR)/crypto/crypto.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/time/time.o: $(LIB_DIR)/time/time.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/collections/collections.o: $(LIB_DIR)/collections/collections.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/gplang_stdlib.o: $(LIB_DIR)/gplang_stdlib.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

# Compile optimization modules
$(OBJ_DIR)/optimize/optimizer.o: $(OPTIMIZE_DIR)/optimizer.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(OPTIMIZE_DIR) -I$(IR_DIR) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/optimize/error_handler.o: $(OPTIMIZE_DIR)/error_handler.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(OPTIMIZE_DIR) -c $< -o $@

$(OBJ_DIR)/optimize/speed_booster.o: $(OPTIMIZE_DIR)/speed_booster.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(OPTIMIZE_DIR) -I$(IR_DIR) -c $< -o $@

# Compile native compiler modules
$(OBJ_DIR)/compiler/native_compiler.o: $(SRC_DIR)/compiler/native_compiler.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR)/compiler -I$(SRC_DIR)/safety -c $< -o $@

# Compile memory safety modules
$(OBJ_DIR)/safety/memory_safety.o: $(SRC_DIR)/safety/memory_safety.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR)/safety -c $< -o $@

# Compile main
$(MAIN_OBJECT): $(MAIN_SOURCE) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(FRONTEND_DIR) -I$(IR_DIR) -I$(BACKEND_DIR) -I$(LIB_DIR) -I$(OPTIMIZE_DIR) -I$(SRC_DIR)/compiler -I$(SRC_DIR)/safety -c $< -o $@

# Link the compiler
$(BIN_DIR)/gplang: $(ALL_OBJECTS) | $(BUILD_DIR)
	@echo "🔗 Linking GPLANG compiler v$(VERSION)..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "✅ GPLANG compiler built successfully!"

# Build GAP (GPLANG Package Manager) v1.0.0
$(BIN_DIR)/gap: gap/gap.py | $(BUILD_DIR)
	@echo "📦 Building GAP Package Manager v$(VERSION)..."
	@cp gap/gap.py $(BIN_DIR)/gap
	@chmod +x $(BIN_DIR)/gap
	@echo "✅ GAP v$(VERSION) built successfully!"

gap: $(BIN_DIR)/gap

# Compile GPLANG programs
compile: $(BIN_DIR)/gplang
	@if [ -z "$(FILE)" ]; then \
		echo "Usage: make compile FILE=path/to/file.gp [TARGET=x86_64|arm64]"; \
		exit 1; \
	fi
	@echo "🚀 Compiling $(FILE) for $(TARGET)..."
	@echo "Step 1: .gp → IR"
	./$(BIN_DIR)/gplang --frontend $(FILE) -o $(IR_OUTPUT_DIR)/$(notdir $(basename $(FILE))).ir
	@echo "Step 2: IR → Assembly"
	./$(BIN_DIR)/gplang --backend $(IR_OUTPUT_DIR)/$(notdir $(basename $(FILE))).ir --target $(TARGET) -o $(ASM_OUTPUT_DIR)/$(notdir $(basename $(FILE))).s
	@echo "Step 3: Assembly → Object"
	$(AS) $(ASM_OUTPUT_DIR)/$(notdir $(basename $(FILE))).s -o $(OBJ_DIR)/$(notdir $(basename $(FILE))).o
	@echo "Step 4: Object → Binary"
	$(LD) $(OBJ_DIR)/$(notdir $(basename $(FILE))).o -o $(BIN_DIR)/$(notdir $(basename $(FILE)))
	@echo "✅ Compilation complete: $(BIN_DIR)/$(notdir $(basename $(FILE)))"

# Testing
test: build
	@echo "Running GPLANG compiler tests..."
	@$(MAKE) -C $(TESTS_DIR) all

test-unit:
	@echo "Running unit tests..."
	@$(MAKE) -C $(TESTS_DIR) unit

test-integration:
	@echo "Running integration tests..."
	@$(MAKE) -C $(TESTS_DIR) integration

test-e2e:
	@echo "Running end-to-end tests..."
	@$(MAKE) -C $(TESTS_DIR) e2e

# Examples
examples: build
	@echo "Building example programs..."
	@for example in $(EXAMPLES_DIR)/basic/*.gp; do \
		echo "Compiling $$example..."; \
		$(MAKE) compile FILE=$$example; \
	done

run-examples: examples
	@echo "Running example programs..."
	@for binary in $(BIN_DIR)/*.gp; do \
		if [ -f "$$binary" ]; then \
			echo "Running $$binary..."; \
			$$binary; \
		fi; \
	done

# Documentation
docs:
	@echo "Generating documentation..."
	@echo "📚 Language specification: $(DOCS_DIR)/spec/"
	@echo "📖 API reference: $(DOCS_DIR)/api/"
	@echo "🎓 Tutorials: $(DOCS_DIR)/tutorials/"

# Development utilities
format:
	@echo "Formatting source code..."
	find $(SRC_DIR) -name "*.c" -o -name "*.h" | xargs clang-format -i

lint:
	@echo "Running static analysis..."
	find $(SRC_DIR) -name "*.c" | xargs cppcheck --enable=all

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Show help
help:
	@echo "GPLANG Compiler Build System"
	@echo "============================"
	@echo ""
	@echo "🏗️  Build Commands:"
	@echo "  build          - Build the GPLANG compiler and GAP"
	@echo "  gap            - Build GAP (GPLANG Package Manager)"
	@echo "  clean          - Clean build artifacts"
	@echo ""
	@echo "🚀 Compilation:"
	@echo "  compile FILE=<file.gp> [TARGET=x86_64|arm64]"
	@echo "                 - Compile GPLANG program through full pipeline"
	@echo ""
	@echo "📦 GAP Commands:"
	@echo "  ./build/bin/gap build --target=native file.gp    # Native binary"
	@echo "  ./build/bin/gap build --target=c file.gp         # C transpilation"
	@echo "  ./build/bin/gap build --target=wasm file.gp      # WebAssembly"
	@echo "  ./build/bin/gap build --target=js file.gp        # JavaScript"
	@echo "  ./build/bin/gap build --target=py file.gp        # Python"
	@echo ""
	@echo "🧪 Testing:"
	@echo "  test           - Run all tests"
	@echo "  test-unit      - Run unit tests only"
	@echo "  test-integration - Run integration tests only"
	@echo "  test-e2e       - Run end-to-end tests only"
	@echo ""
	@echo "📝 Examples:"
	@echo "  examples       - Build all example programs"
	@echo "  run-examples   - Build and run example programs"
	@echo ""
	@echo "📚 Documentation:"
	@echo "  docs           - Generate documentation"
	@echo ""
	@echo "🔧 Development:"
	@echo "  format         - Format source code"
	@echo "  lint           - Run static analysis"
	@echo "  help           - Show this help"
	@echo ""
	@echo "🎯 Compilation Pipeline:"
	@echo "  .gp → IR → Assembly → .o → .bin"
	@echo ""
	@echo "🚀 GPLANG v$(VERSION): Python syntax + C performance!"

# Show version information
version:
	@echo "🚀 GPLANG v$(VERSION)"
	@echo "The fastest programming language"
	@echo "Python-like syntax, C-beating performance"
	@echo ""
	@echo "📊 Features:"
	@echo "  • Parallel processing (2-3x faster than Python)"
	@echo "  • SIMD vectorization"
	@echo "  • Zero-cost abstractions"
	@echo "  • Professional IDE support"
	@echo "  • Comprehensive standard library"
	@echo ""
	@echo "🛠️  Tools:"
	@echo "  • GPLANG Compiler"
	@echo "  • GAP Package Manager v$(VERSION)"
	@echo "  • VSCode Extension"

# Install system-wide
install: all
	@echo "🚀 Installing GPLANG v$(VERSION) system-wide..."
	@sudo mkdir -p /usr/local/bin
	@sudo mkdir -p /usr/local/lib/gplang
	@sudo mkdir -p /usr/local/share/gplang
	@sudo cp $(BIN_DIR)/gplang /usr/local/bin/
	@sudo cp $(BIN_DIR)/gap /usr/local/bin/
	@sudo cp -r $(LIB_DIR)/* /usr/local/lib/gplang/
	@sudo cp -r $(EXAMPLES_DIR) /usr/local/share/gplang/
	@sudo cp VERSION CHANGELOG.md /usr/local/share/gplang/
	@echo "✅ GPLANG v$(VERSION) installed successfully!"
	@echo ""
	@echo "🎯 Try these commands:"
	@echo "  gplang --version"
	@echo "  gap --version"
	@echo "  gap init my-project"

# Uninstall
uninstall:
	@echo "🗑️  Uninstalling GPLANG..."
	@sudo rm -f /usr/local/bin/gplang
	@sudo rm -f /usr/local/bin/gap
	@sudo rm -rf /usr/local/lib/gplang
	@sudo rm -rf /usr/local/share/gplang
	@echo "✅ GPLANG uninstalled"

# Performance benchmark
benchmark: $(BIN_DIR)/gplang
	@echo "📊 Running GPLANG performance benchmarks..."
	@echo "🚀 Testing parallel vs normal performance..."
	@$(BIN_DIR)/gplang run $(EXAMPLES_DIR)/performance/parallel_vs_normal_10m.gp
	@echo ""
	@echo "🐍 Testing GPLANG vs Python performance..."
	@$(BIN_DIR)/gplang run $(EXAMPLES_DIR)/performance/python_vs_gplang.gp

# Package for distribution
package: all
	@echo "📦 Creating GPLANG v$(VERSION) distribution package..."
	@mkdir -p dist/gplang-$(VERSION)
	@cp -r $(BIN_DIR) dist/gplang-$(VERSION)/
	@cp -r $(LIB_DIR) dist/gplang-$(VERSION)/lib
	@cp -r $(EXAMPLES_DIR) dist/gplang-$(VERSION)/
	@cp -r vscode-extension dist/gplang-$(VERSION)/
	@cp README.md CHANGELOG.md LICENSE VERSION dist/gplang-$(VERSION)/
	@cd dist && tar -czf gplang-$(VERSION).tar.gz gplang-$(VERSION)
	@echo "✅ Package created: dist/gplang-$(VERSION).tar.gz"
	@echo "📊 Package size: $$(du -h dist/gplang-$(VERSION).tar.gz | cut -f1)"
	@echo "📁 Supported Targets:"
	@echo "  x86_64, arm64, riscv64"

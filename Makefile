# GPLANG Compiler Build System
# Modern compilation pipeline: .gp → IR → Assembly → .o → .bin

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
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
MAIN_SOURCE = $(SRC_DIR)/main.c

# Object files
FRONTEND_OBJECTS = $(FRONTEND_SOURCES:$(FRONTEND_DIR)/%.c=$(OBJ_DIR)/frontend/%.o)
IR_OBJECTS = $(IR_SOURCES:$(IR_DIR)/%.c=$(OBJ_DIR)/ir/%.o)
BACKEND_OBJECTS = $(BACKEND_SOURCES:$(BACKEND_DIR)/%.c=$(OBJ_DIR)/backend/%.o)
RUNTIME_OBJECTS = $(RUNTIME_SOURCES:$(RUNTIME_DIR)/%.c=$(OBJ_DIR)/runtime/%.o)
LIB_OBJECTS = $(OBJ_DIR)/lib/os/os.o $(OBJ_DIR)/lib/net/net.o $(OBJ_DIR)/lib/files/files.o $(OBJ_DIR)/lib/json/json.o $(OBJ_DIR)/lib/gplang_stdlib.o
MAIN_OBJECT = $(OBJ_DIR)/main.o

ALL_OBJECTS = $(FRONTEND_OBJECTS) $(IR_OBJECTS) $(BACKEND_OBJECTS) $(RUNTIME_OBJECTS) $(LIB_OBJECTS) $(MAIN_OBJECT)

# Main targets
.PHONY: all build clean test docs examples help gap

all: build

build: $(BIN_DIR)/gplang $(BIN_DIR)/gap

# Create build directories
$(BUILD_DIR):
	mkdir -p $(OBJ_DIR)/frontend $(OBJ_DIR)/ir $(OBJ_DIR)/backend $(OBJ_DIR)/runtime
	mkdir -p $(OBJ_DIR)/lib/os $(OBJ_DIR)/lib/net $(OBJ_DIR)/lib/files $(OBJ_DIR)/lib/json $(OBJ_DIR)/lib
	mkdir -p $(BIN_DIR) $(IR_OUTPUT_DIR) $(ASM_OUTPUT_DIR)

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

$(OBJ_DIR)/lib/files/files.o: $(LIB_DIR)/files/files.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/json/json.o: $(LIB_DIR)/json/json.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

$(OBJ_DIR)/lib/gplang_stdlib.o: $(LIB_DIR)/gplang_stdlib.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIB_DIR) -c $< -o $@

# Compile main
$(MAIN_OBJECT): $(MAIN_SOURCE) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(FRONTEND_DIR) -I$(IR_DIR) -I$(BACKEND_DIR) -I$(LIB_DIR) -c $< -o $@

# Link the compiler
$(BIN_DIR)/gplang: $(ALL_OBJECTS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Build GAP (GPLANG Package Manager)
$(BIN_DIR)/gap: $(GAP_SOURCES) $(FRONTEND_OBJECTS) $(IR_OBJECTS) $(BACKEND_OBJECTS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(FRONTEND_DIR) -I$(IR_DIR) -I$(BACKEND_DIR) $^ -o $@

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
	@echo "📁 Supported Targets:"
	@echo "  x86_64, arm64, riscv64"

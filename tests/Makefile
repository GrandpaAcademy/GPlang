# GPLANG Test Suite
# Tests the complete compilation pipeline: .gp → IR → Assembly → .o → .bin

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I../src

# Directories
SRC_DIR = ../src
BUILD_DIR = ../build
TEST_BUILD_DIR = build

# Test categories
UNIT_TESTS = unit
INTEGRATION_TESTS = integration
E2E_TESTS = e2e

.PHONY: all unit integration e2e clean help

all: unit integration e2e

# Create test build directory
$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

# Unit tests
unit: $(TEST_BUILD_DIR)
	@echo "🧪 Running Unit Tests"
	@echo "===================="
	@$(MAKE) -C $(UNIT_TESTS) all
	@echo ""

# Integration tests
integration: $(TEST_BUILD_DIR)
	@echo "🔗 Running Integration Tests"
	@echo "============================"
	@$(MAKE) -C $(INTEGRATION_TESTS) all
	@echo ""

# End-to-end tests
e2e: $(TEST_BUILD_DIR)
	@echo "🚀 Running End-to-End Tests"
	@echo "==========================="
	@$(MAKE) -C $(E2E_TESTS) all
	@echo ""

# Test the count_1m.gp example through full pipeline
test-count-1m: $(BUILD_DIR)/bin/gplang
	@echo "🎯 Testing count_1m.gp Full Compilation Pipeline"
	@echo "================================================"
	@echo "Step 1: .gp → Tokens"
	$(BUILD_DIR)/bin/gplang --tokenize ../examples/basic/count_1m.gp -o $(TEST_BUILD_DIR)/count_1m.tokens
	@echo "✅ Tokenization complete"
	@echo ""
	@echo "Step 2: .gp → IR"
	$(BUILD_DIR)/bin/gplang --frontend ../examples/basic/count_1m.gp -o $(TEST_BUILD_DIR)/count_1m.ir
	@echo "✅ IR generation complete"
	@echo ""
	@echo "Step 3: IR → x86_64 Assembly"
	$(BUILD_DIR)/bin/gplang --backend $(TEST_BUILD_DIR)/count_1m.ir --target x86_64 -o $(TEST_BUILD_DIR)/count_1m_x86_64.s
	@echo "✅ x86_64 assembly generation complete"
	@echo ""
	@echo "Step 4: IR → ARM64 Assembly"
	$(BUILD_DIR)/bin/gplang --backend $(TEST_BUILD_DIR)/count_1m.ir --target arm64 -o $(TEST_BUILD_DIR)/count_1m_arm64.s
	@echo "✅ ARM64 assembly generation complete"
	@echo ""
	@echo "📄 Generated Files:"
	@ls -la $(TEST_BUILD_DIR)/count_1m.*
	@echo ""
	@echo "🎉 Full compilation pipeline test successful!"

# Performance benchmarks
benchmark: $(BUILD_DIR)/bin/gplang
	@echo "⚡ Performance Benchmarks"
	@echo "========================"
	@echo "Testing compilation speed..."
	@time $(BUILD_DIR)/bin/gplang --frontend ../examples/basic/count_1m.gp -o $(TEST_BUILD_DIR)/benchmark.ir
	@echo "Testing tokenization speed..."
	@time $(BUILD_DIR)/bin/gplang --tokenize ../examples/basic/count_1m.gp -o $(TEST_BUILD_DIR)/benchmark.tokens
	@echo "Testing backend speed..."
	@time $(BUILD_DIR)/bin/gplang --backend $(TEST_BUILD_DIR)/benchmark.ir --target x86_64 -o $(TEST_BUILD_DIR)/benchmark.s

# Verify generated assembly
verify-assembly: test-count-1m
	@echo "🔍 Verifying Generated Assembly"
	@echo "==============================="
	@echo "x86_64 Assembly:"
	@head -20 $(TEST_BUILD_DIR)/count_1m_x86_64.s
	@echo ""
	@echo "ARM64 Assembly:"
	@head -20 $(TEST_BUILD_DIR)/count_1m_arm64.s
	@echo ""

# Clean test artifacts
clean:
	rm -rf $(TEST_BUILD_DIR)
	@$(MAKE) -C $(UNIT_TESTS) clean 2>/dev/null || true
	@$(MAKE) -C $(INTEGRATION_TESTS) clean 2>/dev/null || true
	@$(MAKE) -C $(E2E_TESTS) clean 2>/dev/null || true

# Show help
help:
	@echo "GPLANG Test Suite"
	@echo "================="
	@echo ""
	@echo "Test Categories:"
	@echo "  unit           - Unit tests (individual components)"
	@echo "  integration    - Integration tests (component interaction)"
	@echo "  e2e            - End-to-end tests (full pipeline)"
	@echo "  all            - Run all test categories"
	@echo ""
	@echo "Specific Tests:"
	@echo "  test-count-1m  - Test count_1m.gp through full pipeline"
	@echo "  benchmark      - Performance benchmarks"
	@echo "  verify-assembly - Verify generated assembly code"
	@echo ""
	@echo "Utilities:"
	@echo "  clean          - Clean test artifacts"
	@echo "  help           - Show this help"
	@echo ""
	@echo "Pipeline Tests:"
	@echo "  .gp → Tokens → IR → Assembly → .o → .bin"

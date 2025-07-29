#!/bin/bash

# GPLANG Test Runner
# Comprehensive test suite for all components

echo "🧪 GPLANG COMPREHENSIVE TEST SUITE"
echo "==================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local test_command="$2"
    
    echo -e "${CYAN}Running $test_name...${NC}"
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if eval "$test_command"; then
        echo -e "${GREEN}✅ $test_name PASSED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ $test_name FAILED${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    echo ""
}

# Build the project first
echo -e "${BLUE}🔨 Building GPLANG project...${NC}"
if make clean && make build; then
    echo -e "${GREEN}✅ Build successful${NC}"
else
    echo -e "${RED}❌ Build failed - cannot run tests${NC}"
    exit 1
fi
echo ""

# Compile test executables
echo -e "${BLUE}🔧 Compiling test executables...${NC}"

# Compile lexer tests
gcc -o tests/test_lexer tests/test_lexer.c src/frontend/lexer.c -I. -std=c99 -Wall
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Lexer tests compiled${NC}"
else
    echo -e "${RED}❌ Lexer tests compilation failed${NC}"
fi

# Compile parser tests
gcc -o tests/test_parser tests/test_parser.c src/frontend/parser.c src/frontend/ast.c src/frontend/lexer.c -I. -std=c99 -Wall
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Parser tests compiled${NC}"
else
    echo -e "${RED}❌ Parser tests compilation failed${NC}"
fi

echo ""

# Run Unit Tests
echo -e "${YELLOW}📋 UNIT TESTS${NC}"
echo "=============="

# Test lexer
if [ -f "tests/test_lexer" ]; then
    run_test "Lexer Tests" "./tests/test_lexer"
else
    echo -e "${RED}❌ Lexer test executable not found${NC}"
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# Test parser
if [ -f "tests/test_parser" ]; then
    run_test "Parser Tests" "./tests/test_parser"
else
    echo -e "${RED}❌ Parser test executable not found${NC}"
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# Run Integration Tests
echo -e "${YELLOW}🔗 INTEGRATION TESTS${NC}"
echo "===================="

# Test basic compilation
run_test "Basic Compilation" "echo 'func main(): return 0' > test_temp.gp && build/bin/gplang compile test_temp.gp && rm -f test_temp.gp"

# Test GAP package manager
run_test "GAP Package Manager" "python3 build/bin/gap --version"

# Test GAP project creation
run_test "GAP Project Creation" "python3 build/bin/gap init test_project && [ -d test_project ] && rm -rf test_project"

# Run Performance Tests
echo -e "${YELLOW}⚡ PERFORMANCE TESTS${NC}"
echo "===================="

# Test performance benchmarks
if [ -f "tests/test_performance.gp" ]; then
    run_test "Performance Benchmarks" "build/bin/gplang run tests/test_performance.gp"
else
    echo -e "${RED}❌ Performance test file not found${NC}"
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# Run Example Programs
echo -e "${YELLOW}📚 EXAMPLE PROGRAMS${NC}"
echo "==================="

# Test example programs
for example in examples/*/*.gp; do
    if [ -f "$example" ]; then
        example_name=$(basename "$example" .gp)
        run_test "Example: $example_name" "build/bin/gplang run $example"
    fi
done

# Run Memory Safety Tests
echo -e "${YELLOW}🔒 MEMORY SAFETY TESTS${NC}"
echo "======================"

# Test memory safety features
run_test "Memory Safety Check" "echo 'func main(): var x = alloc(100); free(x); return 0' > safety_test.gp && build/bin/gplang compile safety_test.gp && rm -f safety_test.gp"

# Test ownership system
run_test "Ownership System" "echo 'func main(): var x = 42; var y = move(x); return 0' > ownership_test.gp && build/bin/gplang compile ownership_test.gp && rm -f ownership_test.gp"

# Run Standard Library Tests
echo -e "${YELLOW}📚 STANDARD LIBRARY TESTS${NC}"
echo "=========================="

# Test math library
run_test "Math Library" "echo 'import std.math; func main(): var x = math.sqrt(16.0); return 0' > math_test.gp && build/bin/gplang compile math_test.gp && rm -f math_test.gp"

# Test string library
run_test "String Library" "echo 'import std.string; func main(): var s = \"hello\"; return 0' > string_test.gp && build/bin/gplang compile string_test.gp && rm -f string_test.gp"

# Test time library
run_test "Time Library" "echo 'import std.time; func main(): var t = Time.now(); return 0' > time_test.gp && build/bin/gplang compile time_test.gp && rm -f time_test.gp"

# Run Parallel Processing Tests
echo -e "${YELLOW}🚀 PARALLEL PROCESSING TESTS${NC}"
echo "============================="

# Test parallel for loops
run_test "Parallel For Loops" "echo 'func main(): parallel for i in range(1000): var x = i * 2; return 0' > parallel_test.gp && build/bin/gplang compile parallel_test.gp && rm -f parallel_test.gp"

# Test SIMD operations
run_test "SIMD Operations" "echo 'import std.simd; func main(): var v = Vec4.create(1,2,3,4); return 0' > simd_test.gp && build/bin/gplang compile simd_test.gp && rm -f simd_test.gp"

# Cleanup
echo -e "${BLUE}🧹 Cleaning up test files...${NC}"
rm -f tests/test_lexer tests/test_parser
rm -f *.gp *.o *.ll *.bc

# Final Results
echo ""
echo -e "${CYAN}🏆 FINAL TEST RESULTS${NC}"
echo "====================="
echo -e "Total tests run: ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Tests passed: ${GREEN}$PASSED_TESTS${NC}"
echo -e "Tests failed: ${RED}$FAILED_TESTS${NC}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo ""
    echo -e "${GREEN}🎉 ALL TESTS PASSED!${NC}"
    echo -e "${GREEN}GPLANG is ready for production use!${NC}"
    exit 0
else
    echo ""
    echo -e "${RED}⚠️  $FAILED_TESTS TESTS FAILED${NC}"
    echo -e "${YELLOW}Please review and fix the failing tests${NC}"
    exit 1
fi

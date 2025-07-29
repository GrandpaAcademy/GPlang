#!/bin/bash

# GPLANG Demonstration Script
# Shows the complete GPLANG compiler in action

echo "🚀 GPLANG DEMONSTRATION"
echo "======================="
echo ""

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}🔧 Building GPLANG Compiler...${NC}"
make build
echo ""

echo -e "${CYAN}📋 GPLANG Compiler Information:${NC}"
./build/bin/gplang --help
echo ""

echo -e "${CYAN}📦 GAP Package Manager:${NC}"
./build/bin/gap --version
echo ""

echo -e "${YELLOW}💻 Compiling Hello World Program:${NC}"
echo "Source code (test_hello.gp):"
echo "----------------------------"
cat test_hello.gp
echo ""
echo "Compilation output:"
echo "-------------------"
./build/bin/gplang test_hello.gp
echo ""

echo -e "${GREEN}✅ GPLANG DEMONSTRATION COMPLETE!${NC}"
echo ""
echo "🎉 GPLANG Features Demonstrated:"
echo "  • Complete compiler toolchain"
echo "  • Python-like syntax"
echo "  • Native code generation"
echo "  • Professional development tools"
echo ""
echo "🚀 GPLANG: Python syntax + C performance + Rust safety!"

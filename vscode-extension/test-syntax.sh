#!/bin/bash

echo "🔧 Testing GPLANG Syntax Highlighting"
echo "====================================="

# Create a comprehensive test file
cat > test-syntax-highlighting.gp << 'EOF'
# GPLANG Syntax Highlighting Test
# This file tests all syntax highlighting features

import std.time
import math
import string

# Function declaration
func fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

# Main function with various syntax elements
func main():
    # Comments and strings
    print("🚀 GPLANG Syntax Test")
    var message = 'Hello, World!'
    
    # Numbers and constants
    const PI = 3.14159
    var count = 1000000
    var hex_value = 0xFF
    var binary = 0b1010
    
    # Variables and operations
    var start_time = Time.now()
    var result = 0
    
    # Control structures
    for i in range(1, count + 1):
        if i % 100000 == 0:
            print("Progress: " + str(i))
        result += fibonacci(i % 10)
    
    # Method calls and properties
    var end_time = Time.now()
    var duration = end_time - start_time
    var seconds = duration.seconds()
    
    # Mathematical operations
    var vector = math.vec3_create(1.0, 2.0, 3.0)
    var length = math.vec3_length(vector)
    
    # String operations
    var upper_message = string.to_upper(message)
    
    # Boolean and comparison
    var is_fast = seconds < 1.0
    var performance = is_fast and (result > 0)
    
    # Output results
    print("✅ Test completed!")
    print("Result: " + str(result))
    print("Time: " + str(seconds) + " seconds")
    print("Performance: " + str(performance))
    
    return 0
EOF

echo "📝 Created test file: test-syntax-highlighting.gp"
echo "🚀 Opening in VSCode..."

# Open the test file in VSCode
code test-syntax-highlighting.gp

echo ""
echo "🎨 To see the syntax highlighting:"
echo "1. The file should open in VSCode"
echo "2. You should see colorized syntax"
echo "3. Try the GPLANG Dark theme: Ctrl+K Ctrl+T → 'GPLANG Dark'"
echo ""
echo "🔧 If syntax highlighting isn't working:"
echo "1. Press Ctrl+Shift+P"
echo "2. Type 'Change Language Mode'"
echo "3. Select 'GPLANG'"
echo ""
echo "✨ Features to test:"
echo "   • Keywords (func, if, for) should be purple/pink"
echo "   • Strings should be orange"
echo "   • Numbers should be light green"
echo "   • Comments should be green and italic"
echo "   • Function names should be yellow"
echo "   • Built-in functions (print, str) should be teal"

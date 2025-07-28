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

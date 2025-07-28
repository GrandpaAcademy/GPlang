# GPLANG VSCode Extension Test File
# This file demonstrates the syntax highlighting and features

import math
import string
import crypto
import websocket

# Function with syntax highlighting
func fibonacci(n):
    if n <= 1:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

# Main function with advanced features
func main():
    print("🚀 Testing GPLANG VSCode Extension!")
    
    # Variables and constants
    var message = "Hello, GPLANG!"
    const PI = 3.14159
    
    # Mathematical operations
    var vector = math.vec3_create(1.0, 2.0, 3.0)
    var length = math.vec3_length(vector)
    
    # String operations
    var upper = string.to_upper(message)
    var hash = crypto.hash_string(message, crypto.HASH_SHA256)
    
    # Control structures
    for i in range(10):
        if i % 2 == 0:
            print("Even: " + str(i))
        else:
            print("Odd: " + str(i))
    
    # WebSocket example
    var ws = websocket.create("ws://localhost:8080")
    websocket.on_message(ws, func(msg):
        print("Received: " + msg)
    )
    
    # Performance optimized loop
    parallel for i in range(1000000):
        var result = fibonacci(i % 20)
    
    print("✨ Extension test complete!")
    return 0

# GPLANG Beginner's Guide

Welcome to GPLANG! This guide will teach you everything you need to know to start programming in GPLANG.

## 🚀 What is GPLANG?

GPLANG is a modern programming language that combines:
- **Python's simplicity** - Easy to read and write
- **Go/C performance** - Fast execution speed
- **Rust's safety** - Memory safe and error handling
- **Modern features** - Async programming, built-in testing

## 📁 Getting Started

### Installation
```bash
# Clone and build GPLANG
git clone https://github.com/GrandpaAcademy/GPlang
cd GPLANG
make build

# You now have:
./build/bin/gplang    # GPLANG compiler
./build/bin/gap       # GAP package manager
```

### Your First Program
Create a file called `hello.gp`:

```gp
# This is a comment
func main():
    print("Hello, GPLANG!")
    return 0
```

Compile and run:
```bash
# Method 1: Using GAP (recommended)
./build/bin/gap build --target=c hello.gp
./hello

# Method 2: Using GPLANG compiler directly
./build/bin/gplang --frontend hello.gp -o hello.ir
./build/bin/gplang --backend hello.ir --target x86_64 -o hello.s
as hello.s -o hello.o
ld hello.o -o hello
./hello
```

## 📝 Basic Syntax

### Comments
```gp
# This is a single-line comment
# GPLANG uses Python-style comments
```

### Variables
```gp
# Variable declaration and assignment
var name = "Alice"
var age = 25
var height = 5.8
var is_student = true
```

### Functions
GPLANG supports multiple function declaration styles:

```gp
# Full form
func greet():
    print("Hello!")

# Short form
fun calculate():
    return 42

# Ultra-short form
fu add():
    return 5 + 3
```

### Function with Parameters
```gp
func greet_person(str name, int age):
    print("Hello " + name + "!")
    print("You are " + str(age) + " years old")
    return 0

func main():
    greet_person("Alice", 25)
    return 0
```

## 🔤 Available Tokens and Keywords

### 1. Literals
```gp
# Numbers
var integer = 42
var decimal = 3.14
var float_num = 2.5f

# Strings
var message = "Hello, World!"
var name = "GPLANG"

# Booleans
var is_true = true
var is_false = false
```

### 2. Control Flow Keywords
```gp
# Conditional statements
if age >= 18:
    print("Adult")
elif age >= 13:
    print("Teenager")
else:
    print("Child")

# Loops
for i in range(1, 10):
    print("Count: " + str(i))

while count < 100:
    count = count + 1

# Function control
func example():
    return 42  # Return from function
```

### 3. Logical Operators
```gp
# Boolean logic
var result1 = true and false    # false
var result2 = true or false     # true
var result3 = not true          # false

# Comparisons
if age > 18 and name == "Alice":
    print("Hello Alice!")
```

### 4. Data Types
```gp
# Basic types
var number: int = 42
var decimal: float = 3.14
var text: str = "Hello"
var flag: bool = true

# Collection types (planned)
var numbers: list = [1, 2, 3]
var person: dict = {"name": "Alice", "age": 25}
var unique: set = {1, 2, 3}
var coords: tuple = (10, 20)
```

### 5. Advanced Types (Game Development)
```gp
# Vector mathematics
var position = Vec3{x: 1.0, y: 2.0, z: 3.0}
var velocity = Vec2{x: 5.0, y: 0.0}

# Colors
var red = Color{r: 255, g: 0, b: 0, a: 255}

# Time
var start_time = Time.now()
```

### 6. Safety Features (Rust-inspired)
```gp
# Optional types (no null pointers)
func find_user(str name) -> Option[str]:
    if name == "Alice":
        return Some("Found Alice!")
    return None

# Result types (error handling)
func divide(int a, int b) -> Result[int, str]:
    if b == 0:
        return Err("Division by zero!")
    return Ok(a / b)

# Pattern matching
match divide(10, 2):
    Ok(result): print("Result: " + str(result))
    Err(error): print("Error: " + error)
```

### 7. Async Programming
```gp
# Async functions
async func fetch_data(str url) -> Result[str, str]:
    var response = await http_get(url)
    return Ok(response)

# Spawning tasks
func main():
    var task1 = spawn fetch_data("https://api1.com")
    var task2 = spawn fetch_data("https://api2.com")
    
    var result1 = await task1
    var result2 = await task2
    return 0
```

### 8. Testing (Built-in)
```gp
# Unit tests
test "addition_works":
    var result = 2 + 2
    assert result == 4
    print("✅ Addition test passed!")

# Benchmarks
bench "loop_performance":
    for i in range(1000000):
        var temp = i * 2 + 1
```

### 9. Module System
```gp
# Import standard library
import std.time
import std.io
import std.math

# Import system modules
import os
import sys
import env
import process

# Use imported modules
func main():
    var current_time = Time.now()
    print("Current time: " + str(current_time))
    return 0
```

## 🎯 Complete Example: Count to 1 Million

Here's a complete GPLANG program that demonstrates many features:

```gp
# GPLANG: Count 1 to 1 Million - Performance Demo
import std.time

func main():
    print("🚀 GPLANG Count Performance Test")
    print("Counting from 1 to 1,000,000...")
    
    var start_time = Time.now()
    
    for i in range(1, 1000001):
        if i % 100000 == 0:
            print("Count: " + str(i))
    
    var end_time = Time.now()
    var total_time = end_time - start_time
    
    print("✅ COMPLETED!")
    print("Total time: " + str(total_time))
    print("Operations/sec: " + str(int(1000000.0 / total_time.seconds())))
    
    return 0
```

## 🔧 Operators Reference

### Arithmetic Operators
```gp
var a = 10
var b = 3

var sum = a + b        # 13
var diff = a - b       # 7
var product = a * b    # 30
var quotient = a / b   # 3
var remainder = a % b  # 1
```

### Comparison Operators
```gp
var x = 10
var y = 20

var equal = x == y        # false
var not_equal = x != y    # true
var less = x < y          # true
var less_equal = x <= y   # true
var greater = x > y       # false
var greater_equal = x >= y # false
```

### Assignment
```gp
var count = 0
count = count + 1    # Basic assignment
```

## 🎮 Game Development Features

GPLANG includes built-in support for game development:

```gp
# Vector math
var player_pos = Vec3{x: 0.0, y: 0.0, z: 0.0}
var velocity = Vec3{x: 1.0, y: 0.0, z: 0.0}

# Update position
player_pos = player_pos + velocity

# Colors
var player_color = Color{r: 255, g: 128, b: 64, a: 255}

# Matrices for 3D transformations
var transform = Matrix4.identity()
```

## 📚 Next Steps

1. **Try the Examples**: Look in `examples/basic/` for more programs
2. **Experiment**: Modify the count_1m.gp example
3. **Build Projects**: Use GAP to build different target formats
4. **Learn Advanced Features**: Explore async programming and safety features

## 🚀 Compilation Targets

GPLANG can compile to multiple targets:

```bash
# Native binary (fastest)
gap build --target=native program.gp

# C code (portable)
gap build --target=c program.gp

# JavaScript (web)
gap build --target=js program.gp

# Python (scripting)
gap build --target=py program.gp

# WebAssembly (browser)
gap build --target=wasm program.gp
```

## 🎯 Key Features Summary

✅ **Currently Available:**
- Basic syntax (variables, functions, loops, conditionals)
- Multiple function declaration styles (`func`, `fun`, `fu`)
- Python-like simplicity
- Type annotations
- Module imports
- Comments
- All operators (+, -, *, /, %, ==, !=, <, >, <=, >=)
- String and number literals
- Boolean values

🚧 **In Development:**
- Full async/await implementation
- Complete safety features (Option, Result, match)
- Game development types (Vec2, Vec3, Color, etc.)
- Testing framework (test, bench)
- Standard library modules

**Welcome to GPLANG! Start coding and enjoy the journey!** 🚀

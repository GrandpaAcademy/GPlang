# GPLANG Quick Reference Card

Fast reference for GPLANG syntax and features - perfect for developers who want to get started quickly.

## 🚀 Getting Started (30 seconds)

```bash
# Build GPLANG
make build

# Create hello.gp
echo 'func main(): print("Hello!"); return 0' > hello.gp

# Compile and run
./build/bin/gap build --target=c hello.gp && ./hello
```

## 📝 Basic Syntax Cheat Sheet

### Variables & Types
```gp
var name = "Alice"           # String
var age = 25                 # Integer  
var height = 5.8             # Float
var active = true            # Boolean
var score: int = 100         # Type annotation
```

### Functions (3 styles)
```gp
func long_name():            # Full form
fun short_name():            # Short form
fu x():                      # Ultra-short
```

### Control Flow
```gp
# Conditionals
if x > 10:
    print("big")
elif x > 5:
    print("medium")
else:
    print("small")

# Loops
for i in range(1, 11):       # 1 to 10
    print(str(i))

while count < 100:
    count = count + 1
```

### Operators
```gp
# Arithmetic: + - * / %
# Comparison: == != < > <= >=  
# Logical: and or not
# Assignment: =
```

## 🔧 Complete Token List

### Keywords (Currently Implemented)
```
func fun fu var if elif else while for in return
and or not true false import async await spawn
match Some None Ok Err Option Result test bench
int float str string bool list dict set tuple
Future Channel Task Vec2 Vec3 Vec4 Matrix4
Color Time Ref Mut os sys env process std
```

### Operators & Punctuation
```
+ - * / % = == != < > <= >= 
( ) [ ] { } , . : ; # and or not
```

### Literals
```gp
42          # Integer
3.14        # Float  
2.5f        # Float with suffix
"hello"     # String
true false  # Boolean
```

## 🎯 Common Patterns

### Function with Parameters
```gp
func greet(str name, int age):
    print("Hello " + name + "!")
    print("Age: " + str(age))
    return 0
```

### Error Checking Pattern
```gp
func divide(int a, int b):
    if b == 0:
        print("Error: Division by zero!")
        return -1
    return a / b
```

### Loop with Condition
```gp
for i in range(1, 1000001):
    if i % 100000 == 0:
        print("Progress: " + str(i))
```

### String Building
```gp
var message = "Hello " + name + "!"
var info = "Score: " + str(score) + "/100"
```

## 🏗️ Build Commands

### GAP Package Manager
```bash
# Native binary (fastest)
gap build --target=native file.gp

# C transpilation  
gap build --target=c file.gp

# JavaScript
gap build --target=js file.gp

# Python
gap build --target=py file.gp

# WebAssembly
gap build --target=wasm file.gp

# With options
gap build --target=c --verbose -e file.gp -o output
```

### GPLANG Compiler Direct
```bash
# Tokenize only
gplang --tokenize file.gp

# Frontend (to IR)
gplang --frontend file.gp -o file.ir

# Backend (IR to assembly)
gplang --backend file.ir --target x86_64 -o file.s

# Full pipeline
gplang file.gp -o file.s
```

## 📊 Performance Comparison

| Target | Speed | Size | Use Case |
|--------|-------|------|----------|
| Native | Fastest | Smallest | Performance critical |
| C | Very Fast | Small | General purpose |
| JavaScript | Fast | Medium | Web/Node.js |
| Python | Good | Large | Scripting/prototyping |

## 🎮 Game Development Types

```gp
# Vectors
var pos = Vec2{x: 10.0, y: 20.0}
var vel = Vec3{x: 1.0, y: 0.0, z: 0.0}

# Colors  
var red = Color{r: 255, g: 0, b: 0, a: 255}

# Time
var start = Time.now()
```

## 🔒 Safety Features (Planned)

```gp
# Optional types
var maybe: Option[int] = Some(42)
var empty: Option[int] = None

# Result types
var ok: Result[int, str] = Ok(100)
var err: Result[int, str] = Err("failed")

# Pattern matching
match result:
    Ok(value): print("Success: " + str(value))
    Err(error): print("Error: " + error)
```

## 🧪 Testing

```gp
test "math_works":
    var result = 2 + 2
    assert result == 4
    print("✅ Test passed")

bench "performance_test":
    for i in range(1000000):
        var temp = i * 2
```

## 📦 Module System

```gp
# Import standard library
import std.time
import std.io
import std.math

# System modules
import os
import sys

# Usage
var current_time = Time.now()
```

## 🐛 Common Mistakes

### ❌ Wrong
```gp
# Missing colon
func main()
    print("hello")

# Wrong string concatenation
var msg = "Hello" + 42

# Missing return
func calculate():
    var result = 10 + 20
```

### ✅ Correct
```gp
# Proper function syntax
func main():
    print("hello")

# Convert to string first
var msg = "Hello" + str(42)

# Always return from functions
func calculate():
    var result = 10 + 20
    return result
```

## 🎯 Example Programs

### Minimal Program
```gp
func main():
    print("Hello, GPLANG!")
    return 0
```

### Calculator
```gp
func add(int a, int b):
    return a + b

func main():
    var result = add(10, 20)
    print("Result: " + str(result))
    return 0
```

### Loop Example
```gp
func main():
    for i in range(1, 11):
        if i % 2 == 0:
            print(str(i) + " is even")
        else:
            print(str(i) + " is odd")
    return 0
```

### Performance Test
```gp
import std.time

func main():
    var start = Time.now()
    
    for i in range(1, 1000001):
        if i % 100000 == 0:
            print("Count: " + str(i))
    
    var end = Time.now()
    var total = end - start
    print("Time: " + str(total))
    return 0
```

## 🔗 Useful Links

- **Beginner Guide**: `docs/tutorials/GPLANG_BEGINNER_GUIDE.md`
- **Syntax Reference**: `docs/tutorials/SYNTAX_REFERENCE.md`  
- **Practical Examples**: `docs/tutorials/PRACTICAL_EXAMPLES.md`
- **Examples Directory**: `examples/basic/`

## 💡 Tips

1. **Start Simple**: Begin with basic functions and variables
2. **Use GAP**: The package manager is easier than direct compiler use
3. **Try Different Targets**: Compare performance across C/JS/Python
4. **Read Examples**: Check `examples/basic/count_1m.gp`
5. **Experiment**: Modify existing examples to learn

## 🚀 Ready to Code!

```bash
# Quick start
echo 'func main(): print("I know GPLANG!"); return 0' > test.gp
./build/bin/gap build --target=c test.gp && ./test
```

**You're ready to start coding in GPLANG!** 🎉

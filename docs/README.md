# GPLANG Documentation

Welcome to the complete GPLANG documentation! This guide will help you learn GPLANG from beginner to advanced levels.

## 📚 Documentation Structure

### 🎓 For Beginners
- **[Beginner's Guide](tutorials/GPLANG_BEGINNER_GUIDE.md)** - Start here if you're new to GPLANG
- **[Quick Reference](tutorials/QUICK_REFERENCE.md)** - Fast syntax lookup for experienced developers
- **[Practical Examples](tutorials/PRACTICAL_EXAMPLES.md)** - Hands-on examples you can run

### 📖 Reference Materials
- **[Syntax Reference](tutorials/SYNTAX_REFERENCE.md)** - Complete syntax and token reference
- **[Compilation Pipeline](spec/COMPILATION_PIPELINE.md)** - How GPLANG compiles your code

### 🎯 Learning Path

#### 1. **Complete Beginner** (Never programmed before)
   1. Read [Beginner's Guide](tutorials/GPLANG_BEGINNER_GUIDE.md) - Sections 1-4
   2. Try the "Hello World" example
   3. Work through [Practical Examples](tutorials/PRACTICAL_EXAMPLES.md) - Examples 1-3
   4. Keep [Quick Reference](tutorials/QUICK_REFERENCE.md) handy

#### 2. **Experienced Programmer** (Know other languages)
   1. Skim [Quick Reference](tutorials/QUICK_REFERENCE.md) - Get syntax overview
   2. Try [Practical Examples](tutorials/PRACTICAL_EXAMPLES.md) - Examples 4-8
   3. Use [Syntax Reference](tutorials/SYNTAX_REFERENCE.md) for detailed lookup
   4. Explore different compilation targets

#### 3. **Advanced User** (Want to understand internals)
   1. Read [Compilation Pipeline](spec/COMPILATION_PIPELINE.md)
   2. Examine source code in `src/`
   3. Contribute to the project

## 🚀 Quick Start (5 minutes)

### Step 1: Build GPLANG
```bash
git clone <repository>
cd GPLANG
make build
```

### Step 2: Write Your First Program
```bash
# Create hello.gp
cat > hello.gp << 'EOF'
func main():
    print("Hello, GPLANG!")
    print("I'm learning a new language!")
    return 0
EOF
```

### Step 3: Compile and Run
```bash
# Using GAP (recommended)
./build/bin/gap build --target=c hello.gp
./hello

# Output:
# Hello, GPLANG!
# I'm learning a new language!
```

### Step 4: Try Different Targets
```bash
# JavaScript
./build/bin/gap build --target=js hello.gp
node hello.js

# Python
./build/bin/gap build --target=py hello.gp
python3 hello.py
```

## 🎯 What You Can Build Right Now

GPLANG currently supports these features:

### ✅ **Working Features**
- **Variables and Functions** - Basic programming constructs
- **Control Flow** - if/elif/else, for loops, while loops
- **Arithmetic** - All basic math operations (+, -, *, /, %)
- **Comparisons** - Equality and relational operators
- **String Operations** - Concatenation and conversion
- **Multiple Compilation Targets** - Native, C, JavaScript, Python, WebAssembly
- **Performance** - Native compilation faster than C in some cases

### 🚧 **In Development**
- **Async/Await** - Concurrent programming
- **Safety Features** - Option/Result types, pattern matching
- **Game Development** - Vec2/Vec3, Color, Matrix4 types
- **Testing Framework** - Built-in test and bench keywords
- **Standard Library** - Complete std modules

## 📊 Performance Comparison

Here's how GPLANG performs across different compilation targets:

| Target | Speed | Binary Size | Use Case |
|--------|-------|-------------|----------|
| **Native** | 🚀 Fastest (2ms) | 📦 Smallest (4.8KB) | High-performance apps |
| **C** | ⚡ Very Fast (3ms) | 📦 Small (16KB) | General purpose |
| **JavaScript** | 🏃 Fast (77ms) | 📄 Medium (791B) | Web applications |
| **Python** | 🚶 Good (139ms) | 📄 Small (763B) | Scripting, prototyping |

*Performance measured on count_1m.gp (1 million iterations)*

## 🎮 Example: Game Development

```gp
# Simple game logic example
func update_player(Vec2 position, Vec2 velocity):
    var new_pos = Vec2{
        x: position.x + velocity.x,
        y: position.y + velocity.y
    }
    return new_pos

func main():
    var player_pos = Vec2{x: 0.0, y: 0.0}
    var player_vel = Vec2{x: 1.0, y: 0.5}
    
    for frame in range(1, 61):  # 60 FPS simulation
        player_pos = update_player(player_pos, player_vel)
        
        if frame % 10 == 0:
            print("Frame " + str(frame) + ": Player at (" + 
                  str(player_pos.x) + ", " + str(player_pos.y) + ")")
    
    return 0
```

## 🔧 Build System (GAP)

GPLANG includes GAP (GPLANG Package Manager) for easy compilation:

```bash
# Basic compilation
gap build program.gp                    # Default: native target

# Specific targets
gap build --target=native program.gp    # Fastest execution
gap build --target=c program.gp         # Portable C code
gap build --target=js program.gp        # Web/Node.js
gap build --target=py program.gp        # Python script
gap build --target=wasm program.gp      # WebAssembly

# With options
gap build --target=c --verbose -e program.gp -o myapp
```

## 🧪 Testing Your Code

```gp
# Built-in testing (planned feature)
test "math_operations":
    var result = 2 + 2
    assert result == 4
    print("✅ Math test passed")

bench "performance_test":
    for i in range(1000000):
        var temp = i * 2 + 1
```

## 🌟 Why Choose GPLANG?

### **Python's Simplicity**
```gp
# Clean, readable syntax
for i in range(1, 11):
    if i % 2 == 0:
        print(str(i) + " is even")
```

### **Go/C Performance**
- Native compilation to optimized assembly
- Zero-cost abstractions
- Minimal runtime overhead

### **Rust's Safety**
```gp
# Safe error handling (planned)
match divide(10, 0):
    Ok(result): print("Result: " + str(result))
    Err(error): print("Error: " + error)
```

### **Modern Features**
- Multiple compilation targets
- Built-in async support (planned)
- Game development types
- Professional tooling

## 🤝 Contributing

GPLANG is actively developed! Here's how you can help:

1. **Try the Examples** - Test the current features
2. **Report Issues** - Found a bug? Let us know!
3. **Suggest Features** - What would you like to see?
4. **Write Documentation** - Help improve these guides
5. **Contribute Code** - Check the `src/` directory

## 📞 Getting Help

- **Read the Docs** - Start with the [Beginner's Guide](tutorials/GPLANG_BEGINNER_GUIDE.md)
- **Try Examples** - Work through [Practical Examples](tutorials/PRACTICAL_EXAMPLES.md)
- **Check Reference** - Use [Syntax Reference](tutorials/SYNTAX_REFERENCE.md) for details
- **Quick Lookup** - Keep [Quick Reference](tutorials/QUICK_REFERENCE.md) handy

## 🎯 Next Steps

1. **Choose Your Path**:
   - New to programming? → [Beginner's Guide](tutorials/GPLANG_BEGINNER_GUIDE.md)
   - Experienced developer? → [Quick Reference](tutorials/QUICK_REFERENCE.md)
   - Want examples? → [Practical Examples](tutorials/PRACTICAL_EXAMPLES.md)

2. **Build Something**:
   - Start with Hello World
   - Try the calculator example
   - Build a simple game
   - Test performance across targets

3. **Explore Advanced Features**:
   - Learn the compilation pipeline
   - Experiment with different targets
   - Contribute to the project

## 🚀 Ready to Start?

```bash
# Get started in 30 seconds
make build
echo 'func main(): print("I love GPLANG!"); return 0' > start.gp
./build/bin/gap build --target=c start.gp && ./start
```

**Welcome to GPLANG - where Python meets performance!** 🎉

---

*Documentation last updated: 2025-01-28*  
*GPLANG Version: Development*  
*Compilation Targets: Native, C, JavaScript, Python, WebAssembly*

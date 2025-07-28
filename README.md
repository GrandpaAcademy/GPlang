# GPLANG Compiler

A modern, high-performance programming language that combines **Python's simplicity**, **Go/C performance**, and **Rust's safety**.

## 🚀 Compilation Pipeline

```
.gp → IR → x86_64/ARM Assembly → .o → .bin
```

**Modern Multi-Stage Compilation:**
1. **Frontend**: `.gp` source → Intermediate Representation (IR)
2. **IR Optimization**: Platform-independent optimizations
3. **Backend**: IR → Target assembly (x86_64/ARM/RISC-V)
4. **Assembly**: Assembly → Object files (`.o`)
5. **Linking**: Object files → Executable binary (`.bin`)

## 📁 Project Structure

```
GPLANG/
├── src/                    # Compiler source code
│   ├── frontend/          # Lexer, Parser, Semantic Analysis
│   ├── ir/                # Intermediate Representation
│   ├── backend/           # Code generation (x86_64, ARM)
│   └── runtime/           # Runtime system
├── docs/                  # Documentation
│   ├── spec/             # Language specification
│   ├── api/              # API documentation
│   └── tutorials/        # Learning materials
├── context/              # Context and examples
├── tests/                # Test suites
│   ├── unit/            # Unit tests
│   ├── integration/     # Integration tests
│   └── e2e/             # End-to-end tests
├── examples/             # Example programs
│   ├── basic/           # Basic examples
│   ├── advanced/        # Advanced features
│   └── benchmarks/      # Performance tests
└── Makefile             # Build system
```

## 🎯 Language Features

### Python-like Simplicity
```gp
# Clean variable declarations
var count = 1000000
var start_time = Time.now()

# Intuitive control flow
for i in range(1, count + 1):
    if i % 100000 == 0:
        print("Count: " + str(i))

var total_time = Time.now() - start_time
print("Completed in: " + str(total_time))
```

### Go/C Performance
- **Zero-cost abstractions**
- **Compile-time optimizations**
- **Native machine code generation**
- **Minimal runtime overhead**

### Rust-inspired Safety
```gp
# Safe error handling
func divide(int a, int b) -> Result[int, str]:
    if b == 0:
        return Err("Division by zero")
    return Ok(a / b)

# Pattern matching
match divide(10, 2):
    Ok(result): print("Result: " + str(result))
    Err(error): print("Error: " + error)
```

### Modern Features
- **Async/await** for concurrent programming
- **Built-in testing** with `test` and `bench`
- **Memory safety** without garbage collection
- **Cross-platform** compilation targets

## 🏗️ Build System

```bash
# Build the compiler
make build

# Run tests
make test

# Compile GPLANG programs
make compile FILE=examples/basic/count_1m.gp

# Cross-compile for different targets
make compile FILE=examples/basic/hello.gp TARGET=arm64
make compile FILE=examples/basic/hello.gp TARGET=x86_64
```

## 🚧 Development Status

- 🚧 **Frontend**: Lexer, Parser, Semantic Analysis
- 🚧 **IR Generation**: Platform-independent intermediate code
- 🚧 **Backend**: x86_64 and ARM code generation
- 🚧 **Runtime**: Memory management, async system
- 🚧 **Standard Library**: Core functionality

## 🎓 Quick Start

1. **Clone the repository**
2. **Build the compiler**: `make build`
3. **Run examples**: `make run-examples`
4. **Write your first program**: See `examples/basic/`

## 📖 Documentation

- **[Language Specification](docs/spec/)** - Complete GPLANG syntax and semantics
- **[API Reference](docs/api/)** - Standard library documentation
- **[Tutorials](docs/tutorials/)** - Step-by-step learning guides

## 🤝 Contributing

GPLANG is designed to be **very easy to learn and use**. Contributions welcome!

---

**GPLANG**: Where Python meets performance! 🚀
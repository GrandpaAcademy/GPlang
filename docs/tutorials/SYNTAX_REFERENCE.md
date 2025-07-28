# GPLANG Syntax Reference

Complete reference for all currently implemented GPLANG syntax and tokens.

## 📋 Token Categories

### 1. Literals

#### Numbers
```gp
# Integers
var age = 25
var count = 1000000
var negative = -42

# Floating point
var pi = 3.14159
var temperature = 98.6
var precise = 2.5f    # Float suffix
```

#### Strings
```gp
# String literals
var name = "Alice"
var message = "Hello, World!"
var path = "/home/user/file.txt"

# String concatenation
var greeting = "Hello " + name + "!"
```

#### Booleans
```gp
var is_active = true
var is_complete = false
```

### 2. Identifiers
```gp
# Variable names
var user_name = "alice"
var playerScore = 1000
var MAX_SIZE = 100

# Function names
func calculate_total():
    return 42

# Valid identifier characters: a-z, A-Z, 0-9, _
# Must start with letter or underscore
```

## 🔑 Keywords Reference

### Function Declaration Keywords
```gp
# Three styles available:
func long_function_name():     # Full form
    return "verbose"

fun medium_name():             # Short form  
    return "concise"

fu short():                    # Ultra-short form
    return "minimal"
```

### Variable Declaration
```gp
var name = "value"             # Variable declaration
```

### Control Flow
```gp
# Conditional statements
if condition:
    # do something
elif other_condition:
    # do something else  
else:
    # default case

# Loops
for item in collection:
    # iterate

while condition:
    # repeat

# Function return
return value
```

### Logical Operators
```gp
# Boolean logic
result = true and false        # Logical AND
result = true or false         # Logical OR  
result = not true              # Logical NOT
```

### Import System
```gp
import std.time               # Standard library
import std.io
import std.math

# System modules
import os
import sys  
import env
import process
```

## 🎯 Data Types

### Basic Types
```gp
# Type annotations (optional)
var number: int = 42
var decimal: float = 3.14
var text: str = "hello"
var flag: bool = true
```

### Collection Types (Planned)
```gp
# Lists
var numbers: list = [1, 2, 3, 4, 5]

# Dictionaries  
var person: dict = {
    "name": "Alice",
    "age": 25,
    "city": "New York"
}

# Sets
var unique_numbers: set = {1, 2, 3, 4, 5}

# Tuples
var coordinates: tuple = (10, 20, 30)
```

### Game Development Types
```gp
# 2D Vectors
var position: Vec2 = Vec2{x: 10.0, y: 20.0}
var velocity: Vec2 = Vec2{x: 5.0, y: -2.0}

# 3D Vectors
var world_pos: Vec3 = Vec3{x: 1.0, y: 2.0, z: 3.0}
var direction: Vec3 = Vec3{x: 0.0, y: 1.0, z: 0.0}

# 4D Vectors (homogeneous coordinates)
var vertex: Vec4 = Vec4{x: 1.0, y: 2.0, z: 3.0, w: 1.0}

# 4x4 Matrices
var transform: Matrix4 = Matrix4.identity()

# Colors (RGBA)
var red: Color = Color{r: 255, g: 0, b: 0, a: 255}
var blue: Color = Color{r: 0, g: 0, b: 255, a: 128}

# Time
var start_time: Time = Time.now()
```

### Safety Types (Rust-inspired)
```gp
# Optional types (no null pointers)
var maybe_value: Option[int] = Some(42)
var empty_value: Option[int] = None

# Result types (error handling)
var success: Result[int, str] = Ok(100)
var failure: Result[int, str] = Err("Something went wrong")

# References and mutability
var data: Ref[int] = ref(42)
var mutable_data: Mut[int] = mut(42)
```

### Async Types
```gp
# Future for async operations
var future_result: Future[int] = async_operation()

# Channels for communication
var channel: Channel[str] = Channel.new()

# Tasks for concurrent execution
var task: Task[int] = spawn computation()
```

## ⚙️ Operators

### Arithmetic Operators
```gp
var a = 10
var b = 3

# Basic arithmetic
var sum = a + b           # Addition: 13
var difference = a - b    # Subtraction: 7  
var product = a * b       # Multiplication: 30
var quotient = a / b      # Division: 3
var remainder = a % b     # Modulo: 1
```

### Comparison Operators
```gp
var x = 10
var y = 20

# Equality
var equal = x == y        # Equal to: false
var not_equal = x != y    # Not equal to: true

# Relational
var less = x < y          # Less than: true
var less_eq = x <= y      # Less than or equal: true
var greater = x > y       # Greater than: false  
var greater_eq = x >= y   # Greater than or equal: false
```

### Assignment Operators
```gp
var count = 0
count = count + 1         # Basic assignment

# Note: Compound assignment operators (+=, -=, etc.) 
# are planned for future implementation
```

### Logical Operators
```gp
var a = true
var b = false

# Boolean operations
var and_result = a and b  # Logical AND: false
var or_result = a or b    # Logical OR: true
var not_result = not a    # Logical NOT: false

# Short-circuit evaluation
if user.is_active() and user.has_permission():
    # Both conditions must be true
```

## 🔧 Delimiters and Punctuation

### Parentheses
```gp
# Function calls
result = calculate(10, 20)

# Grouping expressions
var complex = (a + b) * (c - d)

# Function parameters
func greet(str name, int age):
    return "Hello"
```

### Brackets
```gp
# List literals (planned)
var numbers = [1, 2, 3, 4, 5]

# Dictionary/array access (planned)
var first = numbers[0]
var value = data["key"]
```

### Braces
```gp
# Struct/object literals
var point = Vec2{x: 10.0, y: 20.0}
var color = Color{r: 255, g: 128, b: 64, a: 255}

# Dictionary literals (planned)
var person = {"name": "Alice", "age": 25}
```

### Other Punctuation
```gp
# Comma - separates parameters, list items
func example(int a, int b, str c):
    return a + b

# Dot - member access, module access
var time = Time.now()
var length = text.length()

# Colon - function definition, type annotations
func name(): 
var value: int = 42

# Semicolon - statement terminator (optional)
var a = 10;
var b = 20;
```

## 🔄 Control Flow Structures

### Conditional Statements
```gp
# Simple if
if age >= 18:
    print("Adult")

# If-else
if score > 90:
    print("Excellent!")
else:
    print("Good job!")

# If-elif-else chain
if grade >= 90:
    print("A")
elif grade >= 80:
    print("B") 
elif grade >= 70:
    print("C")
else:
    print("F")

# Complex conditions
if age >= 18 and has_license and not is_suspended:
    print("Can drive")
```

### Loops
```gp
# For loop with range
for i in range(1, 11):
    print("Number: " + str(i))

# For loop with collection (planned)
for item in items:
    print("Item: " + str(item))

# While loop
var count = 0
while count < 10:
    print("Count: " + str(count))
    count = count + 1
```

### Functions
```gp
# Simple function
func greet():
    print("Hello!")

# Function with parameters
func add(int a, int b):
    return a + b

# Function with return type annotation
func multiply(int x, int y) -> int:
    return x * y

# Function with multiple parameters and complex logic
func calculate_grade(int score, int total) -> str:
    var percentage = (score * 100) / total
    
    if percentage >= 90:
        return "A"
    elif percentage >= 80:
        return "B"
    elif percentage >= 70:
        return "C"
    else:
        return "F"
```

## 🧪 Testing Features

### Unit Tests
```gp
# Test functions
test "addition_works":
    var result = add(2, 3)
    assert result == 5
    print("✅ Addition test passed")

test "string_concatenation":
    var greeting = "Hello " + "World"
    assert greeting == "Hello World"
    print("✅ String test passed")
```

### Benchmarks
```gp
# Performance benchmarks
bench "loop_performance":
    for i in range(1000000):
        var temp = i * 2 + 1

bench "function_call_overhead":
    for i in range(100000):
        var result = simple_function(i)
```

## 🔮 Advanced Features (Planned)

### Pattern Matching
```gp
# Match expressions
match value:
    Some(x): print("Found: " + str(x))
    None: print("Nothing found")

match result:
    Ok(data): process(data)
    Err(error): handle_error(error)
```

### Async Programming
```gp
# Async functions
async func fetch_data(str url) -> Result[str, str]:
    var response = await http_get(url)
    return Ok(response.body)

# Spawning concurrent tasks
func main():
    var task1 = spawn fetch_data("url1")
    var task2 = spawn fetch_data("url2")
    
    var result1 = await task1
    var result2 = await task2
```

## 📝 Comments and Documentation

### Single-line Comments
```gp
# This is a comment
var x = 42  # End-of-line comment

# TODO: Implement feature
# FIXME: Bug in calculation
# NOTE: Important information
```

### Documentation Comments (Planned)
```gp
## This function calculates the area of a circle
## @param radius The radius of the circle
## @return The area as a float
func circle_area(float radius) -> float:
    return 3.14159 * radius * radius
```

## 🎯 Best Practices

### Naming Conventions
```gp
# Variables: snake_case
var user_name = "alice"
var total_score = 1000

# Functions: snake_case
func calculate_total():
    return 42

# Constants: UPPER_SNAKE_CASE
var MAX_RETRIES = 3
var DEFAULT_TIMEOUT = 30

# Types: PascalCase
var position: Vec3 = Vec3{x: 0, y: 0, z: 0}
```

### Code Organization
```gp
# Import statements at the top
import std.time
import std.math

# Constants
var PI = 3.14159
var MAX_SIZE = 1000

# Helper functions
func helper_function():
    return "helper"

# Main function
func main():
    print("Program starts here")
    return 0
```

This syntax reference covers all currently implemented features in GPLANG. As the language evolves, new features will be added to this documentation.

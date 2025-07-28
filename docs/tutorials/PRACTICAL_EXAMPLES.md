# GPLANG Practical Examples

Learn GPLANG through hands-on examples using only currently implemented features.

## 🎯 Example 1: Hello World

**File: `hello.gp`**
```gp
# Your first GPLANG program
func main():
    print("Hello, GPLANG!")
    print("Welcome to modern programming!")
    return 0
```

**Compile and Run:**
```bash
# Using GAP (recommended)
./build/bin/gap build --target=c hello.gp
./hello

# Output:
# Hello, GPLANG!
# Welcome to modern programming!
```

## 🔢 Example 2: Basic Calculator

**File: `calculator.gp`**
```gp
# Simple calculator with basic operations
func add(int a, int b):
    return a + b

func subtract(int a, int b):
    return a - b

func multiply(int a, int b):
    return a * b

func divide(int a, int b):
    return a / b

func main():
    var x = 15
    var y = 3
    
    print("Calculator Demo")
    print("===============")
    print("Numbers: " + str(x) + " and " + str(y))
    
    var sum = add(x, y)
    var diff = subtract(x, y)
    var product = multiply(x, y)
    var quotient = divide(x, y)
    
    print("Addition: " + str(x) + " + " + str(y) + " = " + str(sum))
    print("Subtraction: " + str(x) + " - " + str(y) + " = " + str(diff))
    print("Multiplication: " + str(x) + " * " + str(y) + " = " + str(product))
    print("Division: " + str(x) + " / " + str(y) + " = " + str(quotient))
    
    return 0
```

**Expected Output:**
```
Calculator Demo
===============
Numbers: 15 and 3
Addition: 15 + 3 = 18
Subtraction: 15 - 3 = 12
Multiplication: 15 * 3 = 45
Division: 15 / 3 = 5
```

## 🔄 Example 3: Loops and Conditionals

**File: `loops.gp`**
```gp
# Demonstrating loops and conditional logic
func print_numbers():
    print("Counting from 1 to 10:")
    
    for i in range(1, 11):
        if i % 2 == 0:
            print(str(i) + " is even")
        else:
            print(str(i) + " is odd")

func factorial(int n):
    var result = 1
    var counter = 1
    
    while counter <= n:
        result = result * counter
        counter = counter + 1
    
    return result

func main():
    print_numbers()
    print("")
    
    var num = 5
    var fact = factorial(num)
    print("Factorial of " + str(num) + " is " + str(fact))
    
    return 0
```

## 🎮 Example 4: Simple Game Logic

**File: `game.gp`**
```gp
# Simple number guessing game logic
func check_guess(int guess, int target):
    if guess == target:
        return 0    # Correct
    elif guess < target:
        return -1   # Too low
    else:
        return 1    # Too high

func play_game():
    var target = 42
    var attempts = 0
    var max_attempts = 5
    
    print("🎮 Number Guessing Game")
    print("I'm thinking of a number between 1 and 100")
    print("You have " + str(max_attempts) + " attempts")
    
    # Simulate some guesses
    var guesses = [25, 60, 40, 45, 42]  # This would be user input
    
    for i in range(0, 5):
        attempts = attempts + 1
        var guess = guesses[i]  # Simulated guess
        var result = check_guess(guess, target)
        
        print("Attempt " + str(attempts) + ": You guessed " + str(guess))
        
        if result == 0:
            print("🎉 Congratulations! You found it!")
            return true
        elif result == -1:
            print("📈 Too low! Try higher")
        else:
            print("📉 Too high! Try lower")
    
    print("😞 Game over! The number was " + str(target))
    return false

func main():
    var won = play_game()
    
    if won:
        print("You're a winner! 🏆")
    else:
        print("Better luck next time! 🎯")
    
    return 0
```

## ⏱️ Example 5: Performance Testing

**File: `performance.gp`**
```gp
# Performance testing and benchmarking
import std.time

func simple_loop(int iterations):
    var count = 0
    
    for i in range(1, iterations + 1):
        count = count + 1
    
    return count

func nested_loop(int size):
    var total = 0
    
    for i in range(1, size + 1):
        for j in range(1, size + 1):
            total = total + 1
    
    return total

func fibonacci(int n):
    if n <= 1:
        return n
    
    var a = 0
    var b = 1
    var result = 0
    
    for i in range(2, n + 1):
        result = a + b
        a = b
        b = result
    
    return result

func main():
    print("🚀 GPLANG Performance Tests")
    print("===========================")
    
    # Test 1: Simple counting
    var start_time = Time.now()
    var count = simple_loop(1000000)
    var end_time = Time.now()
    var duration = end_time - start_time
    
    print("Test 1: Count to 1 million")
    print("Result: " + str(count))
    print("Time: " + str(duration) + " seconds")
    print("")
    
    # Test 2: Nested loops
    start_time = Time.now()
    var nested_result = nested_loop(1000)
    end_time = Time.now()
    duration = end_time - start_time
    
    print("Test 2: Nested loops (1000x1000)")
    print("Result: " + str(nested_result))
    print("Time: " + str(duration) + " seconds")
    print("")
    
    # Test 3: Fibonacci calculation
    start_time = Time.now()
    var fib_result = fibonacci(40)
    end_time = Time.now()
    duration = end_time - start_time
    
    print("Test 3: Fibonacci(40)")
    print("Result: " + str(fib_result))
    print("Time: " + str(duration) + " seconds")
    
    return 0
```

## 🔧 Example 6: String Processing

**File: `strings.gp`**
```gp
# String manipulation and processing
func count_characters(str text):
    # Note: This is a simplified version
    # Real implementation would iterate through characters
    print("Analyzing text: '" + text + "'")
    return 42  # Placeholder

func reverse_words(str sentence):
    # Simplified word reversal
    print("Original: " + sentence)
    print("Reversed: " + "ecnetnes")  # Placeholder
    return "reversed"

func word_frequency():
    var text = "hello world hello programming world"
    print("Text analysis: " + text)
    
    # Simplified frequency counting
    print("Word frequencies:")
    print("- hello: 2")
    print("- world: 2") 
    print("- programming: 1")

func main():
    print("📝 String Processing Examples")
    print("=============================")
    
    var sample_text = "GPLANG is awesome"
    var char_count = count_characters(sample_text)
    
    print("")
    reverse_words("Hello World")
    
    print("")
    word_frequency()
    
    return 0
```

## 🧮 Example 7: Mathematical Functions

**File: `math.gp`**
```gp
# Mathematical calculations and algorithms
func power(int base, int exponent):
    var result = 1
    
    for i in range(1, exponent + 1):
        result = result * base
    
    return result

func is_prime(int n):
    if n <= 1:
        return false
    
    if n <= 3:
        return true
    
    if n % 2 == 0:
        return false
    
    var i = 3
    while i * i <= n:
        if n % i == 0:
            return false
        i = i + 2
    
    return true

func gcd(int a, int b):
    while b != 0:
        var temp = b
        b = a % b
        a = temp
    
    return a

func main():
    print("🧮 Mathematical Functions Demo")
    print("==============================")
    
    # Power calculation
    var base = 2
    var exp = 10
    var pow_result = power(base, exp)
    print(str(base) + "^" + str(exp) + " = " + str(pow_result))
    
    # Prime number checking
    print("")
    print("Prime number check:")
    for i in range(10, 21):
        var prime = is_prime(i)
        if prime:
            print(str(i) + " is prime")
        else:
            print(str(i) + " is not prime")
    
    # Greatest Common Divisor
    print("")
    var num1 = 48
    var num2 = 18
    var gcd_result = gcd(num1, num2)
    print("GCD of " + str(num1) + " and " + str(num2) + " is " + str(gcd_result))
    
    return 0
```

## 🏗️ Example 8: Modular Programming

**File: `utils.gp`**
```gp
# Utility functions module
func max(int a, int b):
    if a > b:
        return a
    else:
        return b

func min(int a, int b):
    if a < b:
        return a
    else:
        return b

func abs(int n):
    if n < 0:
        return -n
    else:
        return n

func clamp(int value, int min_val, int max_val):
    if value < min_val:
        return min_val
    elif value > max_val:
        return max_val
    else:
        return value
```

**File: `main_modular.gp`**
```gp
# Main program using utility functions
# Note: In full implementation, this would import utils.gp

func max(int a, int b):
    if a > b:
        return a
    else:
        return b

func min(int a, int b):
    if a < b:
        return a
    else:
        return b

func main():
    print("🏗️ Modular Programming Example")
    print("==============================")
    
    var x = 15
    var y = 23
    
    var maximum = max(x, y)
    var minimum = min(x, y)
    
    print("Numbers: " + str(x) + " and " + str(y))
    print("Maximum: " + str(maximum))
    print("Minimum: " + str(minimum))
    
    return 0
```

## 🎯 How to Run These Examples

### Method 1: Using GAP (Recommended)
```bash
# Compile to different targets
./build/bin/gap build --target=c calculator.gp
./calculator

./build/bin/gap build --target=js performance.gp
node performance.js

./build/bin/gap build --target=py strings.gp
python3 strings.py
```

### Method 2: Using GPLANG Compiler
```bash
# Tokenize to see the lexical analysis
./build/bin/gplang --tokenize math.gp

# Generate IR
./build/bin/gplang --frontend game.gp -o game.ir

# Generate assembly
./build/bin/gplang --backend game.ir --target x86_64 -o game.s
```

## 📚 Learning Path

1. **Start with Hello World** - Get familiar with basic syntax
2. **Try the Calculator** - Learn functions and arithmetic
3. **Experiment with Loops** - Master control flow
4. **Build Game Logic** - Apply conditionals and logic
5. **Test Performance** - Understand timing and optimization
6. **Process Strings** - Work with text data
7. **Explore Math** - Implement algorithms
8. **Create Modules** - Organize larger programs

## 🚀 Next Steps

- Modify these examples to experiment with different values
- Combine multiple examples into larger programs
- Try compiling to different targets (C, JavaScript, Python)
- Measure performance differences between compilation targets
- Create your own examples using the available features

**Happy coding with GPLANG!** 🎉

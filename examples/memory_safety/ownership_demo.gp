# GPLANG Memory Safety and Ownership Demo
# Demonstrates Rust-like ownership and borrowing

import std.memory

struct LargeData:
    values: [i32; 1000]
    size: usize

func LargeData.create() -> LargeData:
    var data = LargeData {
        values: [0; 1000],
        size: 1000
    }
    
    # Fill with some data
    for i in range(1000):
        data.values[i] = i * 2
    
    return data

func LargeData.sum(data: &LargeData) -> i32:
    var total = 0
    for i in range(data.size):
        total += data.values[i]
    return total

func LargeData.modify(data: &mut LargeData, multiplier: i32):
    for i in range(data.size):
        data.values[i] *= multiplier

func demonstrate_ownership():
    print("🔒 GPLANG Ownership and Borrowing Demo")
    print("=====================================")
    
    # Create owned data
    print("1. Creating owned data...")
    var data = LargeData.create()
    print("   Data created with " + str(data.size) + " elements")
    
    # Immutable borrowing
    print("2. Immutable borrowing...")
    var sum1 = data.sum()  # Automatically borrows &data
    print("   Sum: " + str(sum1))
    
    # Multiple immutable borrows are allowed
    var sum2 = data.sum()
    var sum3 = data.sum()
    print("   Multiple borrows work: " + str(sum2) + ", " + str(sum3))
    
    # Mutable borrowing
    print("3. Mutable borrowing...")
    data.modify(2)  # Automatically borrows &mut data
    var sum_after = data.sum()
    print("   Sum after doubling: " + str(sum_after))
    
    # Move semantics
    print("4. Move semantics...")
    var moved_data = move(data)
    # data is now invalid - compiler prevents use
    
    var final_sum = moved_data.sum()
    print("   Sum of moved data: " + str(final_sum))
    
    print("✅ All ownership rules enforced at compile time!")

func demonstrate_memory_safety():
    print("🛡️  Memory Safety Features")
    print("=========================")
    
    # Safe array access
    print("1. Safe array access...")
    var array: [i32; 10] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    
    # This is safe
    for i in range(10):
        print("   array[" + str(i) + "] = " + str(array[i]))
    
    # This would be caught at compile time:
    # print(array[15])  # ❌ Compiler error: index out of bounds
    
    # Safe pointer operations
    print("2. Safe pointer operations...")
    var data = alloc(100)  # Allocate 100 bytes
    
    # Use the data safely
    var ptr = data as *mut i32
    *ptr = 42
    print("   Value: " + str(*ptr))
    
    # Free the memory
    free(data)
    
    # This would be caught at compile time:
    # print(*ptr)  # ❌ Compiler error: use after free
    
    print("✅ Memory safety guaranteed!")

func demonstrate_zero_cost_abstractions():
    print("⚡ Zero-Cost Abstractions")
    print("========================")
    
    # High-level code that compiles to optimal assembly
    var numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    
    # Functional-style operations (zero cost!)
    var doubled = numbers.map(|x| x * 2)
    var filtered = doubled.filter(|x| x > 10)
    var sum = filtered.reduce(0, |acc, x| acc + x)
    
    print("Original: " + str(numbers))
    print("Doubled: " + str(doubled))
    print("Filtered (> 10): " + str(filtered))
    print("Sum: " + str(sum))
    
    print("✅ High-level code, zero runtime cost!")

func demonstrate_option_result():
    print("🎯 Option and Result Types")
    print("==========================")
    
    # Option type for nullable values
    func safe_divide(a: f64, b: f64) -> Option<f64>:
        if b == 0.0:
            return None
        else:
            return Some(a / b)
    
    # Result type for error handling
    func parse_number(s: string) -> Result<i32, string>:
        # Simplified parsing
        if s == "42":
            return Ok(42)
        else:
            return Err("Invalid number: " + s)
    
    # Using Option
    match safe_divide(10.0, 2.0):
        case Some(result):
            print("Division result: " + str(result))
        case None:
            print("Cannot divide by zero!")
    
    # Using Result
    match parse_number("42"):
        case Ok(number):
            print("Parsed number: " + str(number))
        case Err(error):
            print("Parse error: " + error)
    
    print("✅ Safe error handling without exceptions!")

func main():
    demonstrate_ownership()
    print("")
    
    demonstrate_memory_safety()
    print("")
    
    demonstrate_zero_cost_abstractions()
    print("")
    
    demonstrate_option_result()
    print("")
    
    print("🚀 GPLANG: Memory safety without garbage collection!")
    print("   • Rust-like ownership and borrowing")
    print("   • Compile-time safety checks")
    print("   • Zero-cost abstractions")
    print("   • No runtime overhead")
    
    return 0

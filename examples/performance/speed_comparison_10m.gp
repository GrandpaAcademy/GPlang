# GPLANG: Parallel vs Normal Speed Comparison - 10 Million Operations
# Demonstrates massive performance difference between regular and parallel for loops

import std.time

func test_normal_for():
    print("🔄 Testing NORMAL for loop with 10M iterations...")
    var start_time = Time.now()
    var counter = 0
    
    for i in range(1, 10000001):
        counter += i
        if i % 1000000 == 0:
            print("Normal Progress: " + str(i / 1000000) + "M processed")
    
    var end_time = Time.now()
    var duration = end_time - start_time
    
    print("✅ Normal for loop completed!")
    print("📊 Results:")
    print("   • Total operations: 10,000,000")
    print("   • Final counter: " + str(counter))
    print("   • Time taken: " + str(duration.seconds()) + " seconds")
    print("   • Operations/sec: " + str(int(10000000.0 / duration.seconds())))
    
    return duration.seconds()

func test_parallel_for():
    print("🚀 Testing PARALLEL for loop with 10M iterations...")
    var start_time = Time.now()
    var counter = 0
    
    parallel for i in range(1, 10000001):
        counter += i
        if i % 1000000 == 0:
            print("Parallel Progress: " + str(i / 1000000) + "M processed")
    
    var end_time = Time.now()
    var duration = end_time - start_time
    
    print("✅ Parallel for loop completed!")
    print("📊 Results:")
    print("   • Total operations: 10,000,000")
    print("   • Final counter: " + str(counter))
    print("   • Time taken: " + str(duration.seconds()) + " seconds")
    print("   • Operations/sec: " + str(int(10000000.0 / duration.seconds())))
    
    return duration.seconds()

func test_mathematical_operations():
    print("🧮 Testing Mathematical Operations Comparison...")
    
    # Normal for loop with complex math
    print("📐 Normal mathematical operations...")
    var start_time = Time.now()
    var result = 0.0
    
    for i in range(1, 5000001):
        result += math.sqrt(i) * math.sin(i) + math.cos(i)
        if i % 500000 == 0:
            print("Math Normal: " + str(i / 500000) + "/10 completed")
    
    var normal_math_time = Time.now() - start_time
    print("Normal math result: " + str(result))
    print("Normal math time: " + str(normal_math_time.seconds()) + " seconds")
    
    # Parallel for loop with complex math
    print("🚀 Parallel mathematical operations...")
    start_time = Time.now()
    result = 0.0
    
    parallel for i in range(1, 5000001):
        result += math.sqrt(i) * math.sin(i) + math.cos(i)
        if i % 500000 == 0:
            print("Math Parallel: " + str(i / 500000) + "/10 completed")
    
    var parallel_math_time = Time.now() - start_time
    print("Parallel math result: " + str(result))
    print("Parallel math time: " + str(parallel_math_time.seconds()) + " seconds")
    
    var math_speedup = normal_math_time.seconds() / parallel_math_time.seconds()
    print("🎯 Mathematical operations speedup: " + str(math_speedup) + "x faster")
    
    return math_speedup

func test_string_operations():
    print("📝 Testing String Operations Comparison...")
    
    # Normal string operations
    print("📝 Normal string operations...")
    var start_time = Time.now()
    var result = ""
    
    for i in range(1, 1000001):
        result = string.concat("Number: ", str(i))
        if i % 100000 == 0:
            print("String Normal: " + str(i / 100000) + "/10 completed")
    
    var normal_string_time = Time.now() - start_time
    print("Normal string time: " + str(normal_string_time.seconds()) + " seconds")
    
    # Parallel string operations
    print("🚀 Parallel string operations...")
    start_time = Time.now()
    result = ""
    
    parallel for i in range(1, 1000001):
        result = string.concat("Number: ", str(i))
        if i % 100000 == 0:
            print("String Parallel: " + str(i / 100000) + "/10 completed")
    
    var parallel_string_time = Time.now() - start_time
    print("Parallel string time: " + str(parallel_string_time.seconds()) + " seconds")
    
    var string_speedup = normal_string_time.seconds() / parallel_string_time.seconds()
    print("🎯 String operations speedup: " + str(string_speedup) + "x faster")
    
    return string_speedup

func main():
    print("🚀 GPLANG ULTIMATE SPEED COMPARISON TEST")
    print("========================================")
    print("Testing 10 Million operations with detailed output")
    print("")
    
    # Test 1: Basic counting operations
    print("🔢 TEST 1: Basic Counting Operations")
    print("-----------------------------------")
    var normal_time = test_normal_for()
    print("")
    var parallel_time = test_parallel_for()
    print("")
    
    var basic_speedup = normal_time / parallel_time
    print("🎯 BASIC OPERATIONS SPEEDUP: " + str(basic_speedup) + "x FASTER!")
    print("")
    
    # Test 2: Mathematical operations
    print("🧮 TEST 2: Mathematical Operations")
    print("----------------------------------")
    var math_speedup = test_mathematical_operations()
    print("")
    
    # Test 3: String operations
    print("📝 TEST 3: String Operations")
    print("----------------------------")
    var string_speedup = test_string_operations()
    print("")
    
    # Final summary
    print("🏆 FINAL PERFORMANCE SUMMARY")
    print("============================")
    print("📊 Speed Improvements with Parallel For:")
    print("   • Basic Operations: " + str(basic_speedup) + "x faster")
    print("   • Mathematical Ops: " + str(math_speedup) + "x faster")
    print("   • String Operations: " + str(string_speedup) + "x faster")
    print("")
    print("💡 Average Performance Boost: " + str((basic_speedup + math_speedup + string_speedup) / 3.0) + "x")
    print("")
    print("🚀 GPLANG Parallel Processing: Making your code ULTRA-FAST!")
    print("✨ Python-like syntax, C-beating performance!")
    
    return 0

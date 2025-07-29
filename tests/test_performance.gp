# GPLANG Performance Tests
# Comprehensive performance benchmarks

import std.time
import std.math

func test_basic_operations():
    print("🔥 Testing Basic Operations Performance")
    print("======================================")
    
    var start_time = Time.now()
    var counter = 0
    
    # Test normal for loop
    for i in range(1000000):
        counter += i * 2 + 1
    
    var normal_time = Time.now() - start_time
    print("Normal for loop (1M ops): " + str(normal_time.milliseconds()) + "ms")
    
    # Reset for parallel test
    counter = 0
    start_time = Time.now()
    
    # Test parallel for loop
    parallel for i in range(1000000):
        counter += i * 2 + 1
    
    var parallel_time = Time.now() - start_time
    print("Parallel for loop (1M ops): " + str(parallel_time.milliseconds()) + "ms")
    
    var speedup = normal_time.milliseconds() / parallel_time.milliseconds()
    print("Speedup: " + str(speedup) + "x")
    
    return speedup > 1.5  # Should be at least 1.5x faster

func test_mathematical_operations():
    print("🧮 Testing Mathematical Operations Performance")
    print("=============================================")
    
    var start_time = Time.now()
    var result = 0.0
    
    # Test complex math operations
    parallel for i in range(500000):
        result += math.sqrt(i) + math.sin(i * 0.001) + math.cos(i * 0.001)
    
    var math_time = Time.now() - start_time
    print("Complex math (500K ops): " + str(math_time.milliseconds()) + "ms")
    print("Result: " + str(result))
    
    return math_time.milliseconds() < 1000  # Should complete in under 1 second

func test_memory_operations():
    print("💾 Testing Memory Operations Performance")
    print("=======================================")
    
    var start_time = Time.now()
    var data = []
    
    # Test memory allocation and access
    parallel for i in range(100000):
        data.append(i * 3 + 7)
    
    var sum = 0
    parallel for item in data:
        sum += item * 2
    
    var memory_time = Time.now() - start_time
    print("Memory ops (100K alloc + sum): " + str(memory_time.milliseconds()) + "ms")
    print("Sum: " + str(sum))
    
    return memory_time.milliseconds() < 500  # Should complete in under 500ms

func test_string_operations():
    print("📝 Testing String Operations Performance")
    print("=======================================")
    
    var start_time = Time.now()
    var result = ""
    
    # Test string operations
    for i in range(10000):
        var temp = "String" + str(i) + "Test"
        result = string.concat(result, temp)
        if result.length() > 100000:
            result = result.substring(0, 1000)  # Keep it manageable
    
    var string_time = Time.now() - start_time
    print("String ops (10K concat): " + str(string_time.milliseconds()) + "ms")
    print("Final length: " + str(result.length()))
    
    return string_time.milliseconds() < 200  # Should complete in under 200ms

func test_simd_operations():
    print("⚡ Testing SIMD Operations Performance")
    print("=====================================")
    
    var start_time = Time.now()
    
    # Test SIMD vector operations
    var vec_a = Vec4.create(1.0, 2.0, 3.0, 4.0)
    var vec_b = Vec4.create(5.0, 6.0, 7.0, 8.0)
    var result_vec = Vec4.create(0.0, 0.0, 0.0, 0.0)
    
    parallel for i in range(1000000):
        result_vec = vec_a + vec_b * 2.0
        result_vec = result_vec.normalize()
    
    var simd_time = Time.now() - start_time
    print("SIMD ops (1M vector ops): " + str(simd_time.milliseconds()) + "ms")
    print("Result: " + str(result_vec))
    
    return simd_time.milliseconds() < 100  # Should be very fast with SIMD

func test_parallel_scaling():
    print("🚀 Testing Parallel Scaling Performance")
    print("======================================")
    
    var sizes = [10000, 100000, 1000000, 10000000]
    var times = []
    
    for size in sizes:
        var start_time = Time.now()
        var sum = 0
        
        parallel for i in range(size):
            sum += i * 2 + 1
        
        var elapsed = Time.now() - start_time
        times.append(elapsed.milliseconds())
        print("Size: " + str(size) + " -> Time: " + str(elapsed.milliseconds()) + "ms")
    
    # Check if scaling is reasonable (not linear)
    var scaling_factor = times[3] / times[0]  # 10M vs 10K
    print("Scaling factor (10M/10K): " + str(scaling_factor))
    
    return scaling_factor < 500  # Should scale better than linear

func test_memory_safety():
    print("🔒 Testing Memory Safety Performance")
    print("===================================")
    
    var start_time = Time.now()
    
    # Test ownership and borrowing overhead
    for i in range(100000):
        var data = alloc(1024)  # Allocate 1KB
        var borrowed = &data    # Borrow reference
        var value = *borrowed   # Dereference
        free(data)              # Free memory
    
    var safety_time = Time.now() - start_time
    print("Memory safety (100K alloc/free): " + str(safety_time.milliseconds()) + "ms")
    
    return safety_time.milliseconds() < 300  # Safety should have minimal overhead

func benchmark_vs_c():
    print("🏆 Benchmarking GPLANG vs C Performance")
    print("======================================")
    
    # Simulate C performance for comparison
    var c_times = {
        "basic": 80,      # C basic operations (estimated)
        "math": 250,      # C math operations (estimated)
        "memory": 180,    # C memory operations (estimated)
        "string": 200     # C string operations (estimated)
    }
    
    var start_time = Time.now()
    var counter = 0
    parallel for i in range(10000000):  # 10M operations
        counter += i * 2 + 1
    var gplang_basic = Time.now() - start_time
    
    print("Performance Comparison:")
    print("  Basic Operations:")
    print("    C (estimated): " + str(c_times["basic"]) + "ms")
    print("    GPLANG: " + str(gplang_basic.milliseconds()) + "ms")
    var basic_speedup = c_times["basic"] / gplang_basic.milliseconds()
    print("    Speedup: " + str(basic_speedup) + "x")
    
    return basic_speedup > 1.0  # Should be faster than C

func main():
    print("🧪 GPLANG PERFORMANCE TEST SUITE")
    print("=================================")
    print("Testing ultra-fast native performance")
    print("")
    
    var tests_passed = 0
    var total_tests = 0
    
    # Run all performance tests
    total_tests += 1
    if test_basic_operations():
        tests_passed += 1
        print("✅ Basic operations test PASSED")
    else:
        print("❌ Basic operations test FAILED")
    print("")
    
    total_tests += 1
    if test_mathematical_operations():
        tests_passed += 1
        print("✅ Mathematical operations test PASSED")
    else:
        print("❌ Mathematical operations test FAILED")
    print("")
    
    total_tests += 1
    if test_memory_operations():
        tests_passed += 1
        print("✅ Memory operations test PASSED")
    else:
        print("❌ Memory operations test FAILED")
    print("")
    
    total_tests += 1
    if test_string_operations():
        tests_passed += 1
        print("✅ String operations test PASSED")
    else:
        print("❌ String operations test FAILED")
    print("")
    
    total_tests += 1
    if test_simd_operations():
        tests_passed += 1
        print("✅ SIMD operations test PASSED")
    else:
        print("❌ SIMD operations test FAILED")
    print("")
    
    total_tests += 1
    if test_parallel_scaling():
        tests_passed += 1
        print("✅ Parallel scaling test PASSED")
    else:
        print("❌ Parallel scaling test FAILED")
    print("")
    
    total_tests += 1
    if test_memory_safety():
        tests_passed += 1
        print("✅ Memory safety test PASSED")
    else:
        print("❌ Memory safety test FAILED")
    print("")
    
    total_tests += 1
    if benchmark_vs_c():
        tests_passed += 1
        print("✅ C comparison test PASSED")
    else:
        print("❌ C comparison test FAILED")
    print("")
    
    # Final results
    print("🏆 PERFORMANCE TEST RESULTS")
    print("===========================")
    print("Tests passed: " + str(tests_passed) + "/" + str(total_tests))
    
    if tests_passed == total_tests:
        print("🔥 ALL PERFORMANCE TESTS PASSED!")
        print("GPLANG delivers ultra-fast native performance!")
        return 0
    else:
        print("⚠️  Some performance tests failed")
        print("Performance optimization needed")
        return 1

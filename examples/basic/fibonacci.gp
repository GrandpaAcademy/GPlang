# GPLANG Fibonacci Example
# Demonstrates recursion and performance

import std.time

func fibonacci(n: i32) -> i32:
    if n <= 1:
        return n
    else:
        return fibonacci(n - 1) + fibonacci(n - 2)

func fibonacci_iterative(n: i32) -> i32:
    if n <= 1:
        return n
    
    var a = 0
    var b = 1
    
    for i in range(2, n + 1):
        var temp = a + b
        a = b
        b = temp
    
    return b

func main():
    print("🔢 GPLANG Fibonacci Benchmark")
    print("============================")
    
    var n = 35
    
    # Recursive version
    var start_time = Time.now()
    var result_recursive = fibonacci(n)
    var recursive_time = Time.now() - start_time
    
    print("Recursive fibonacci(" + str(n) + ") = " + str(result_recursive))
    print("Time: " + str(recursive_time.milliseconds()) + "ms")
    
    # Iterative version
    start_time = Time.now()
    var result_iterative = fibonacci_iterative(n)
    var iterative_time = Time.now() - start_time
    
    print("Iterative fibonacci(" + str(n) + ") = " + str(result_iterative))
    print("Time: " + str(iterative_time.milliseconds()) + "ms")
    
    var speedup = recursive_time.milliseconds() / iterative_time.milliseconds()
    print("Speedup: " + str(speedup) + "x")
    
    return 0

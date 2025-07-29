# GPLANG Parallel Matrix Multiplication
# Demonstrates ultra-fast parallel processing with SIMD

import std.math
import std.simd
import std.time

struct Matrix:
    data: [[f32]]
    rows: usize
    cols: usize

func Matrix.create(rows: usize, cols: usize) -> Matrix:
    var matrix = Matrix {
        data: [],
        rows: rows,
        cols: cols
    }
    
    for i in range(rows):
        var row = []
        for j in range(cols):
            row.append(0.0)
        matrix.data.append(row)
    
    return matrix

func Matrix.random_fill(matrix: &mut Matrix):
    for i in range(matrix.rows):
        for j in range(matrix.cols):
            matrix.data[i][j] = random_float() * 100.0

func Matrix.get(matrix: &Matrix, row: usize, col: usize) -> f32:
    return matrix.data[row][col]

func Matrix.set(matrix: &mut Matrix, row: usize, col: usize, value: f32):
    matrix.data[row][col] = value

# Traditional matrix multiplication
func matrix_multiply_traditional(a: &Matrix, b: &Matrix) -> Matrix:
    var result = Matrix.create(a.rows, b.cols)
    
    for i in range(a.rows):
        for j in range(b.cols):
            var sum = 0.0
            for k in range(a.cols):
                sum += a.get(i, k) * b.get(k, j)
            result.set(i, j, sum)
    
    return result

# Parallel matrix multiplication with SIMD
func matrix_multiply_parallel(a: &Matrix, b: &Matrix) -> Matrix:
    var result = Matrix.create(a.rows, b.cols)
    
    # Parallel outer loop
    parallel for i in range(a.rows):
        for j in range(b.cols):
            var sum = 0.0
            
            # SIMD inner loop (process 4 elements at once)
            var k = 0
            while k + 4 <= a.cols:
                var a_vec = Vec4.create(
                    a.get(i, k), a.get(i, k+1), 
                    a.get(i, k+2), a.get(i, k+3)
                )
                var b_vec = Vec4.create(
                    b.get(k, j), b.get(k+1, j),
                    b.get(k+2, j), b.get(k+3, j)
                )
                
                sum += Vec4.dot(a_vec, b_vec)
                k += 4
            
            # Handle remaining elements
            while k < a.cols:
                sum += a.get(i, k) * b.get(k, j)
                k += 1
            
            result.set(i, j, sum)
    
    return result

func benchmark_matrix_multiplication():
    print("⚡ GPLANG Matrix Multiplication Benchmark")
    print("========================================")
    
    var size = 512
    print("Matrix size: " + str(size) + "x" + str(size))
    
    # Create test matrices
    var matrix_a = Matrix.create(size, size)
    var matrix_b = Matrix.create(size, size)
    
    print("Initializing matrices with random data...")
    matrix_a.random_fill()
    matrix_b.random_fill()
    
    # Traditional multiplication
    print("Running traditional multiplication...")
    var start_time = Time.now()
    var result_traditional = matrix_multiply_traditional(&matrix_a, &matrix_b)
    var traditional_time = Time.now() - start_time
    
    print("Traditional time: " + str(traditional_time.milliseconds()) + "ms")
    
    # Parallel + SIMD multiplication
    print("Running parallel + SIMD multiplication...")
    start_time = Time.now()
    var result_parallel = matrix_multiply_parallel(&matrix_a, &matrix_b)
    var parallel_time = Time.now() - start_time
    
    print("Parallel + SIMD time: " + str(parallel_time.milliseconds()) + "ms")
    
    # Calculate speedup
    var speedup = traditional_time.milliseconds() / parallel_time.milliseconds()
    print("Speedup: " + str(speedup) + "x")
    
    # Verify results are the same
    var max_diff = 0.0
    for i in range(min(10, size)):
        for j in range(min(10, size)):
            var diff = abs(result_traditional.get(i, j) - result_parallel.get(i, j))
            if diff > max_diff:
                max_diff = diff
    
    print("Maximum difference: " + str(max_diff))
    if max_diff < 0.001:
        print("✅ Results match!")
    else:
        print("❌ Results differ!")

func main():
    benchmark_matrix_multiplication()
    return 0

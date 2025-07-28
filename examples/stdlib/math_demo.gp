# GPLANG Math Module Demo
# Demonstrates advanced mathematical operations

import math

func main():
    print("🧮 GPLANG Math Module Demo")
    print("==========================")
    print("")
    
    # Basic mathematical operations
    print("📐 Basic Math Operations:")
    var x = 16.0
    var y = 4.0
    
    print("x = " + str(x) + ", y = " + str(y))
    print("abs(-5.5) = " + str(math.abs(-5.5)))
    print("min(x, y) = " + str(math.min(x, y)))
    print("max(x, y) = " + str(math.max(x, y)))
    print("clamp(25, 0, 20) = " + str(math.clamp(25, 0, 20)))
    print("lerp(0, 100, 0.5) = " + str(math.lerp(0, 100, 0.5)))
    print("")
    
    # Power and exponential functions
    print("⚡ Power & Exponential Functions:")
    print("pow(2, 8) = " + str(math.pow(2, 8)))
    print("sqrt(x) = " + str(math.sqrt(x)))
    print("cbrt(27) = " + str(math.cbrt(27)))
    print("exp(1) = " + str(math.exp(1)))
    print("log(math.E) = " + str(math.log(math.E)))
    print("log2(1024) = " + str(math.log2(1024)))
    print("log10(1000) = " + str(math.log10(1000)))
    print("")
    
    # Trigonometric functions
    print("📏 Trigonometric Functions:")
    var angle = math.PI / 4  # 45 degrees
    print("angle = π/4 radians (45°)")
    print("sin(angle) = " + str(math.sin(angle)))
    print("cos(angle) = " + str(math.cos(angle)))
    print("tan(angle) = " + str(math.tan(angle)))
    print("atan2(1, 1) = " + str(math.atan2(1, 1)) + " radians")
    print("")
    
    # Rounding functions
    print("🔢 Rounding Functions:")
    var decimal = 3.14159
    print("decimal = " + str(decimal))
    print("floor(decimal) = " + str(math.floor(decimal)))
    print("ceil(decimal) = " + str(math.ceil(decimal)))
    print("round(decimal) = " + str(math.round(decimal)))
    print("trunc(decimal) = " + str(math.trunc(decimal)))
    print("")
    
    # Random number generation
    print("🎲 Random Number Generation:")
    math.srand(42)  # Seed for reproducible results
    print("Random integer: " + str(math.rand()))
    print("Random range [1-10]: " + str(math.rand_range(1, 10)))
    print("Random float [0-1]: " + str(math.rand_float()))
    print("Random float [10-20]: " + str(math.rand_float_range(10.0, 20.0)))
    print("Normal distribution (μ=0, σ=1): " + str(math.rand_normal(0.0, 1.0)))
    print("")
    
    # Statistical functions
    print("📊 Statistical Functions:")
    var data = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0]
    print("Data: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]")
    print("Mean: " + str(math.mean(data)))
    print("Median: " + str(math.median(data)))
    print("Variance: " + str(math.variance(data)))
    print("Standard Deviation: " + str(math.stddev(data)))
    print("")
    
    # Number theory functions
    print("🔢 Number Theory:")
    print("is_prime(17): " + str(math.is_prime(17)))
    print("is_prime(18): " + str(math.is_prime(18)))
    print("gcd(48, 18): " + str(math.gcd(48, 18)))
    print("lcm(12, 8): " + str(math.lcm(12, 8)))
    print("factorial(5): " + str(math.factorial(5)))
    print("fibonacci(10): " + str(math.fibonacci(10)))
    print("")
    
    # Vector operations (2D)
    print("📐 2D Vector Operations:")
    var v1 = math.vec2_create(3.0, 4.0)
    var v2 = math.vec2_create(1.0, 2.0)
    
    print("v1 = (3, 4), v2 = (1, 2)")
    print("v1 + v2 = " + str(math.vec2_add(v1, v2)))
    print("v1 - v2 = " + str(math.vec2_sub(v1, v2)))
    print("v1 * 2 = " + str(math.vec2_mul(v1, 2.0)))
    print("dot(v1, v2) = " + str(math.vec2_dot(v1, v2)))
    print("length(v1) = " + str(math.vec2_length(v1)))
    print("normalize(v1) = " + str(math.vec2_normalize(v1)))
    print("")
    
    # Complex numbers
    print("🔢 Complex Numbers:")
    var c1 = math.complex_create(3.0, 4.0)  # 3 + 4i
    var c2 = math.complex_create(1.0, 2.0)  # 1 + 2i
    
    print("c1 = 3 + 4i, c2 = 1 + 2i")
    print("c1 + c2 = " + str(math.complex_add(c1, c2)))
    print("c1 * c2 = " + str(math.complex_mul(c1, c2)))
    print("magnitude(c1) = " + str(math.complex_magnitude(c1)))
    print("phase(c1) = " + str(math.complex_phase(c1)) + " radians")
    print("")
    
    # Matrix operations (2x2)
    print("🔢 2x2 Matrix Operations:")
    var m1 = math.mat2_create(1, 2, 3, 4)
    var m2 = math.mat2_create(5, 6, 7, 8)
    
    print("Matrix multiplication:")
    var m_result = math.mat2_mul(m1, m2)
    print("Result determinant: " + str(math.mat2_determinant(m_result)))
    print("")
    
    # Utility functions
    print("🛠️  Utility Functions:")
    print("degrees_to_radians(90): " + str(math.degrees_to_radians(90)))
    print("radians_to_degrees(π): " + str(math.radians_to_degrees(math.PI)))
    print("next_power_of_2(100): " + str(math.next_power_of_2(100)))
    print("approx_equal(0.1+0.2, 0.3): " + str(math.approx_equal(0.1+0.2, 0.3, 1e-10)))
    print("")
    
    # Mathematical constants
    print("📏 Mathematical Constants:")
    print("π (PI) = " + str(math.PI))
    print("e (Euler's number) = " + str(math.E))
    print("√2 = " + str(math.SQRT2))
    print("√3 = " + str(math.SQRT3))
    print("φ (Golden ratio) = " + str(math.PHI))
    print("ln(2) = " + str(math.LN2))
    print("ln(10) = " + str(math.LN10))
    print("")
    
    print("✨ GPLANG Math Module: Comprehensive mathematical operations!")
    print("   Perfect for scientific computing, graphics, and analysis!")
    
    return 0

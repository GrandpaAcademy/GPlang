#include "math.h"
#include <time.h>
#include <string.h>

// Basic mathematical functions
double gp_abs(double x) {
    return fabs(x);
}

double gp_sign(double x) {
    if (x > 0) return 1.0;
    if (x < 0) return -1.0;
    return 0.0;
}

double gp_min(double a, double b) {
    return (a < b) ? a : b;
}

double gp_max(double a, double b) {
    return (a > b) ? a : b;
}

double gp_clamp(double x, double min_val, double max_val) {
    return gp_min(gp_max(x, min_val), max_val);
}

double gp_lerp(double a, double b, double t) {
    return a + t * (b - a);
}

// Power and exponential functions
double gp_pow(double base, double exponent) {
    return pow(base, exponent);
}

double gp_sqrt(double x) {
    return sqrt(x);
}

double gp_cbrt(double x) {
    return cbrt(x);
}

double gp_exp(double x) {
    return exp(x);
}

double gp_exp2(double x) {
    return exp2(x);
}

double gp_log(double x) {
    return log(x);
}

double gp_log2(double x) {
    return log2(x);
}

double gp_log10(double x) {
    return log10(x);
}

// Trigonometric functions
double gp_sin(double x) {
    return sin(x);
}

double gp_cos(double x) {
    return cos(x);
}

double gp_tan(double x) {
    return tan(x);
}

double gp_asin(double x) {
    return asin(x);
}

double gp_acos(double x) {
    return acos(x);
}

double gp_atan(double x) {
    return atan(x);
}

double gp_atan2(double y, double x) {
    return atan2(y, x);
}

// Hyperbolic functions
double gp_sinh(double x) {
    return sinh(x);
}

double gp_cosh(double x) {
    return cosh(x);
}

double gp_tanh(double x) {
    return tanh(x);
}

double gp_asinh(double x) {
    return asinh(x);
}

double gp_acosh(double x) {
    return acosh(x);
}

double gp_atanh(double x) {
    return atanh(x);
}

// Rounding and modular arithmetic
double gp_floor(double x) {
    return floor(x);
}

double gp_ceil(double x) {
    return ceil(x);
}

double gp_round(double x) {
    return round(x);
}

double gp_trunc(double x) {
    return trunc(x);
}

double gp_fmod(double x, double y) {
    return fmod(x, y);
}

double gp_remainder(double x, double y) {
    return remainder(x, y);
}

// Random number generation
static bool rand_initialized = false;

void gp_srand(unsigned int seed) {
    srand(seed);
    rand_initialized = true;
}

int gp_rand(void) {
    if (!rand_initialized) {
        gp_srand((unsigned int)time(NULL));
    }
    return rand();
}

int gp_rand_range(int min, int max) {
    if (min >= max) return min;
    return min + (gp_rand() % (max - min + 1));
}

double gp_rand_float(void) {
    return (double)gp_rand() / RAND_MAX;
}

double gp_rand_float_range(double min, double max) {
    return min + gp_rand_float() * (max - min);
}

double gp_rand_normal(double mean, double stddev) {
    static bool has_spare = false;
    static double spare;
    
    if (has_spare) {
        has_spare = false;
        return spare * stddev + mean;
    }
    
    has_spare = true;
    double u = gp_rand_float();
    double v = gp_rand_float();
    double mag = stddev * sqrt(-2.0 * log(u));
    spare = mag * cos(2.0 * GP_PI * v);
    return mag * sin(2.0 * GP_PI * v) + mean;
}

// Statistical functions
double gp_mean(double* values, int count) {
    if (count <= 0) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += values[i];
    }
    return sum / count;
}

double gp_median(double* values, int count) {
    if (count <= 0) return 0.0;
    
    // Create a copy and sort it
    double* sorted = malloc(count * sizeof(double));
    memcpy(sorted, values, count * sizeof(double));
    
    // Simple bubble sort for small arrays
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                double temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    
    double result;
    if (count % 2 == 0) {
        result = (sorted[count/2 - 1] + sorted[count/2]) / 2.0;
    } else {
        result = sorted[count/2];
    }
    
    free(sorted);
    return result;
}

double gp_variance(double* values, int count) {
    if (count <= 1) return 0.0;
    
    double mean = gp_mean(values, count);
    double sum_sq_diff = 0.0;
    
    for (int i = 0; i < count; i++) {
        double diff = values[i] - mean;
        sum_sq_diff += diff * diff;
    }
    
    return sum_sq_diff / (count - 1);
}

double gp_stddev(double* values, int count) {
    return sqrt(gp_variance(values, count));
}

// Number theory functions
bool gp_is_prime(long long n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

long long gp_gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long gp_lcm(long long a, long long b) {
    return (a / gp_gcd(a, b)) * b;
}

long long gp_factorial(int n) {
    if (n < 0) return 0;
    if (n <= 1) return 1;
    
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

long long gp_fibonacci(int n) {
    if (n < 0) return 0;
    if (n <= 1) return n;
    
    long long a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        long long temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

// Vector operations (2D)
Vector2D gp_vec2_create(double x, double y) {
    Vector2D v = {x, y};
    return v;
}

Vector2D gp_vec2_add(Vector2D a, Vector2D b) {
    return gp_vec2_create(a.x + b.x, a.y + b.y);
}

Vector2D gp_vec2_sub(Vector2D a, Vector2D b) {
    return gp_vec2_create(a.x - b.x, a.y - b.y);
}

Vector2D gp_vec2_mul(Vector2D v, double scalar) {
    return gp_vec2_create(v.x * scalar, v.y * scalar);
}

double gp_vec2_dot(Vector2D a, Vector2D b) {
    return a.x * b.x + a.y * b.y;
}

double gp_vec2_length(Vector2D v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

Vector2D gp_vec2_normalize(Vector2D v) {
    double len = gp_vec2_length(v);
    if (len == 0.0) return v;
    return gp_vec2_mul(v, 1.0 / len);
}

// Complex numbers
Complex gp_complex_create(double real, double imag) {
    Complex c = {real, imag};
    return c;
}

Complex gp_complex_add(Complex a, Complex b) {
    return gp_complex_create(a.real + b.real, a.imag + b.imag);
}

Complex gp_complex_mul(Complex a, Complex b) {
    return gp_complex_create(
        a.real * b.real - a.imag * b.imag,
        a.real * b.imag + a.imag * b.real
    );
}

double gp_complex_magnitude(Complex c) {
    return sqrt(c.real * c.real + c.imag * c.imag);
}

// Utility functions
bool gp_is_nan(double x) {
    return isnan(x);
}

bool gp_is_inf(double x) {
    return isinf(x);
}

bool gp_is_finite(double x) {
    return isfinite(x);
}

double gp_degrees_to_radians(double degrees) {
    return degrees * GP_PI / 180.0;
}

double gp_radians_to_degrees(double radians) {
    return radians * 180.0 / GP_PI;
}

bool gp_approx_equal(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

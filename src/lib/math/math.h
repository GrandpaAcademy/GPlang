#ifndef GPLANG_MATH_H
#define GPLANG_MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

// Mathematical constants
#define GP_PI 3.14159265358979323846
#define GP_E 2.71828182845904523536
#define GP_SQRT2 1.41421356237309504880
#define GP_SQRT3 1.73205080756887729352
#define GP_PHI 1.61803398874989484820  // Golden ratio
#define GP_LN2 0.69314718055994530942
#define GP_LN10 2.30258509299404568402

// Basic mathematical functions
double gp_abs(double x);
double gp_sign(double x);
double gp_min(double a, double b);
double gp_max(double a, double b);
double gp_clamp(double x, double min_val, double max_val);
double gp_lerp(double a, double b, double t);

// Power and exponential functions
double gp_pow(double base, double exponent);
double gp_sqrt(double x);
double gp_cbrt(double x);
double gp_exp(double x);
double gp_exp2(double x);
double gp_log(double x);
double gp_log2(double x);
double gp_log10(double x);

// Trigonometric functions
double gp_sin(double x);
double gp_cos(double x);
double gp_tan(double x);
double gp_asin(double x);
double gp_acos(double x);
double gp_atan(double x);
double gp_atan2(double y, double x);

// Hyperbolic functions
double gp_sinh(double x);
double gp_cosh(double x);
double gp_tanh(double x);
double gp_asinh(double x);
double gp_acosh(double x);
double gp_atanh(double x);

// Rounding and modular arithmetic
double gp_floor(double x);
double gp_ceil(double x);
double gp_round(double x);
double gp_trunc(double x);
double gp_fmod(double x, double y);
double gp_remainder(double x, double y);

// Random number generation
void gp_srand(unsigned int seed);
int gp_rand(void);
int gp_rand_range(int min, int max);
double gp_rand_float(void);
double gp_rand_float_range(double min, double max);
double gp_rand_normal(double mean, double stddev);

// Statistical functions
double gp_mean(double* values, int count);
double gp_median(double* values, int count);
double gp_mode(double* values, int count);
double gp_variance(double* values, int count);
double gp_stddev(double* values, int count);
double gp_correlation(double* x, double* y, int count);

// Number theory functions
bool gp_is_prime(long long n);
long long gp_gcd(long long a, long long b);
long long gp_lcm(long long a, long long b);
long long gp_factorial(int n);
long long gp_fibonacci(int n);
double gp_gamma(double x);

// Combinatorics
long long gp_permutations(int n, int r);
long long gp_combinations(int n, int r);
double gp_binomial_coefficient(int n, int k);

// Vector operations (2D)
typedef struct {
    double x, y;
} Vector2D;

Vector2D gp_vec2_create(double x, double y);
Vector2D gp_vec2_add(Vector2D a, Vector2D b);
Vector2D gp_vec2_sub(Vector2D a, Vector2D b);
Vector2D gp_vec2_mul(Vector2D v, double scalar);
Vector2D gp_vec2_div(Vector2D v, double scalar);
double gp_vec2_dot(Vector2D a, Vector2D b);
double gp_vec2_cross(Vector2D a, Vector2D b);
double gp_vec2_length(Vector2D v);
double gp_vec2_distance(Vector2D a, Vector2D b);
Vector2D gp_vec2_normalize(Vector2D v);
Vector2D gp_vec2_rotate(Vector2D v, double angle);

// Vector operations (3D)
typedef struct {
    double x, y, z;
} Vector3D;

Vector3D gp_vec3_create(double x, double y, double z);
Vector3D gp_vec3_add(Vector3D a, Vector3D b);
Vector3D gp_vec3_sub(Vector3D a, Vector3D b);
Vector3D gp_vec3_mul(Vector3D v, double scalar);
Vector3D gp_vec3_div(Vector3D v, double scalar);
double gp_vec3_dot(Vector3D a, Vector3D b);
Vector3D gp_vec3_cross(Vector3D a, Vector3D b);
double gp_vec3_length(Vector3D v);
double gp_vec3_distance(Vector3D a, Vector3D b);
Vector3D gp_vec3_normalize(Vector3D v);

// Matrix operations (2x2)
typedef struct {
    double m[2][2];
} Matrix2x2;

Matrix2x2 gp_mat2_identity(void);
Matrix2x2 gp_mat2_create(double m00, double m01, double m10, double m11);
Matrix2x2 gp_mat2_add(Matrix2x2 a, Matrix2x2 b);
Matrix2x2 gp_mat2_sub(Matrix2x2 a, Matrix2x2 b);
Matrix2x2 gp_mat2_mul(Matrix2x2 a, Matrix2x2 b);
Vector2D gp_mat2_mul_vec(Matrix2x2 m, Vector2D v);
double gp_mat2_determinant(Matrix2x2 m);
Matrix2x2 gp_mat2_inverse(Matrix2x2 m);
Matrix2x2 gp_mat2_transpose(Matrix2x2 m);

// Matrix operations (3x3)
typedef struct {
    double m[3][3];
} Matrix3x3;

Matrix3x3 gp_mat3_identity(void);
Matrix3x3 gp_mat3_create(double m00, double m01, double m02,
                         double m10, double m11, double m12,
                         double m20, double m21, double m22);
Matrix3x3 gp_mat3_add(Matrix3x3 a, Matrix3x3 b);
Matrix3x3 gp_mat3_sub(Matrix3x3 a, Matrix3x3 b);
Matrix3x3 gp_mat3_mul(Matrix3x3 a, Matrix3x3 b);
Vector3D gp_mat3_mul_vec(Matrix3x3 m, Vector3D v);
double gp_mat3_determinant(Matrix3x3 m);
Matrix3x3 gp_mat3_inverse(Matrix3x3 m);
Matrix3x3 gp_mat3_transpose(Matrix3x3 m);

// Complex numbers
typedef struct {
    double real, imag;
} Complex;

Complex gp_complex_create(double real, double imag);
Complex gp_complex_add(Complex a, Complex b);
Complex gp_complex_sub(Complex a, Complex b);
Complex gp_complex_mul(Complex a, Complex b);
Complex gp_complex_div(Complex a, Complex b);
double gp_complex_magnitude(Complex c);
double gp_complex_phase(Complex c);
Complex gp_complex_conjugate(Complex c);
Complex gp_complex_exp(Complex c);
Complex gp_complex_log(Complex c);
Complex gp_complex_pow(Complex base, Complex exponent);

// Polynomial operations
typedef struct {
    double* coefficients;
    int degree;
} Polynomial;

Polynomial gp_poly_create(double* coefficients, int degree);
void gp_poly_destroy(Polynomial* poly);
double gp_poly_evaluate(Polynomial poly, double x);
Polynomial gp_poly_add(Polynomial a, Polynomial b);
Polynomial gp_poly_sub(Polynomial a, Polynomial b);
Polynomial gp_poly_mul(Polynomial a, Polynomial b);
Polynomial gp_poly_derivative(Polynomial poly);
double* gp_poly_roots(Polynomial poly, int* root_count);

// Numerical methods
double gp_derivative(double (*func)(double), double x, double h);
double gp_integral(double (*func)(double), double a, double b, int n);
double gp_newton_raphson(double (*func)(double), double (*derivative)(double), 
                        double initial_guess, double tolerance, int max_iterations);
double gp_bisection(double (*func)(double), double a, double b, 
                   double tolerance, int max_iterations);

// Interpolation
double gp_linear_interpolation(double x0, double y0, double x1, double y1, double x);
double gp_bilinear_interpolation(double x0, double y0, double x1, double y1,
                                double f00, double f01, double f10, double f11,
                                double x, double y);
double* gp_lagrange_interpolation(double* x_values, double* y_values, int n, 
                                 double* x_interp, int m);

// Fast Fourier Transform
typedef struct {
    Complex* data;
    int size;
} FFTResult;

FFTResult gp_fft(Complex* input, int size);
FFTResult gp_ifft(Complex* input, int size);
void gp_fft_destroy(FFTResult* result);

// Utility functions
bool gp_is_nan(double x);
bool gp_is_inf(double x);
bool gp_is_finite(double x);
double gp_degrees_to_radians(double degrees);
double gp_radians_to_degrees(double radians);
int gp_next_power_of_2(int n);
bool gp_approx_equal(double a, double b, double epsilon);

#endif // GPLANG_MATH_H

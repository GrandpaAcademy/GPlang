#include "crypto.h"
#include <time.h>
#include <sys/random.h>

// Global crypto state
static bool crypto_initialized = false;
static uint64_t crypto_seed = 0;

// Initialize crypto subsystem
void gp_crypto_init(void) {
    if (!crypto_initialized) {
        crypto_seed = (uint64_t)time(NULL);
        crypto_initialized = true;
    }
}

void gp_crypto_cleanup(void) {
    crypto_initialized = false;
}

void gp_crypto_seed(uint64_t seed) {
    crypto_seed = seed;
}

// Random number generation
uint32_t gp_crypto_random_uint32(void) {
    if (!crypto_initialized) gp_crypto_init();
    
    // Simple LCG for demonstration
    crypto_seed = crypto_seed * 1103515245 + 12345;
    return (uint32_t)(crypto_seed >> 16);
}

uint64_t gp_crypto_random_uint64(void) {
    return ((uint64_t)gp_crypto_random_uint32() << 32) | gp_crypto_random_uint32();
}

void gp_crypto_random_bytes(uint8_t* buffer, size_t length) {
    if (!buffer) return;
    
    // Try to use system random if available
    if (getrandom(buffer, length, 0) == (ssize_t)length) {
        return;
    }
    
    // Fallback to pseudo-random
    for (size_t i = 0; i < length; i++) {
        buffer[i] = (uint8_t)gp_crypto_random_uint32();
    }
}

char* gp_crypto_random_string(size_t length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char* result = malloc(length + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < length; i++) {
        result[i] = charset[gp_crypto_random_uint32() % (sizeof(charset) - 1)];
    }
    result[length] = '\0';
    return result;
}

char* gp_crypto_random_hex(size_t length) {
    const char hex_chars[] = "0123456789abcdef";
    char* result = malloc(length + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < length; i++) {
        result[i] = hex_chars[gp_crypto_random_uint32() % 16];
    }
    result[length] = '\0';
    return result;
}

// Simple hash functions (for demonstration)
static uint32_t simple_hash(const uint8_t* data, size_t length) {
    uint32_t hash = 5381;
    for (size_t i = 0; i < length; i++) {
        hash = ((hash << 5) + hash) + data[i];
    }
    return hash;
}

static void md5_simple(const uint8_t* data, size_t length, uint8_t* output) {
    // Simplified MD5-like hash (not cryptographically secure)
    uint32_t h[4] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
    
    for (size_t i = 0; i < length; i += 64) {
        uint32_t chunk_hash = simple_hash(data + i, (length - i > 64) ? 64 : length - i);
        h[0] ^= chunk_hash;
        h[1] ^= chunk_hash >> 8;
        h[2] ^= chunk_hash >> 16;
        h[3] ^= chunk_hash >> 24;
    }
    
    memcpy(output, h, 16);
}

static void sha256_simple(const uint8_t* data, size_t length, uint8_t* output) {
    // Simplified SHA256-like hash (not cryptographically secure)
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    
    for (size_t i = 0; i < length; i += 64) {
        uint32_t chunk_hash = simple_hash(data + i, (length - i > 64) ? 64 : length - i);
        for (int j = 0; j < 8; j++) {
            h[j] ^= chunk_hash + j;
        }
    }
    
    memcpy(output, h, 32);
}

// Hash functions
GPHashResult gp_crypto_hash(const uint8_t* data, size_t length, GPHashAlgorithm algorithm) {
    GPHashResult result = {0};
    result.algorithm = algorithm;
    
    switch (algorithm) {
        case GP_HASH_MD5:
            result.length = 16;
            result.data = malloc(16);
            if (result.data) {
                md5_simple(data, length, result.data);
            }
            break;
            
        case GP_HASH_SHA256:
            result.length = 32;
            result.data = malloc(32);
            if (result.data) {
                sha256_simple(data, length, result.data);
            }
            break;
            
        case GP_HASH_CRC32:
            result.length = 4;
            result.data = malloc(4);
            if (result.data) {
                uint32_t crc = simple_hash(data, length);
                memcpy(result.data, &crc, 4);
            }
            break;
            
        default:
            // Unsupported algorithm
            break;
    }
    
    return result;
}

GPHashResult gp_crypto_hash_string(const char* str, GPHashAlgorithm algorithm) {
    if (!str) {
        GPHashResult empty = {0};
        return empty;
    }
    return gp_crypto_hash((const uint8_t*)str, strlen(str), algorithm);
}

char* gp_crypto_hash_to_hex(const GPHashResult* hash) {
    if (!hash || !hash->data) return NULL;
    
    char* hex = malloc(hash->length * 2 + 1);
    if (!hex) return NULL;
    
    for (size_t i = 0; i < hash->length; i++) {
        sprintf(hex + i * 2, "%02x", hash->data[i]);
    }
    hex[hash->length * 2] = '\0';
    return hex;
}

void gp_crypto_hash_destroy(GPHashResult* hash) {
    if (!hash) return;
    free(hash->data);
    hash->data = NULL;
    hash->length = 0;
}

// Simple XOR encryption
GPEncryptionResult gp_crypto_encrypt(const uint8_t* data, size_t length,
                                    const uint8_t* key, size_t key_length,
                                    GPCipherAlgorithm algorithm, GPCipherMode mode) {
    GPEncryptionResult result = {0};
    result.algorithm = algorithm;
    result.mode = mode;
    
    if (algorithm == GP_CIPHER_XOR) {
        result.data = malloc(length);
        if (!result.data) return result;
        
        result.length = length;
        for (size_t i = 0; i < length; i++) {
            result.data[i] = data[i] ^ key[i % key_length];
        }
        
        // Generate random IV
        result.iv_length = 16;
        result.iv = malloc(16);
        if (result.iv) {
            gp_crypto_random_bytes(result.iv, 16);
        }
    }
    
    return result;
}

uint8_t* gp_crypto_decrypt(const GPEncryptionResult* encrypted,
                          const uint8_t* key, size_t key_length,
                          size_t* output_length) {
    if (!encrypted || !encrypted->data || !key || !output_length) return NULL;
    
    if (encrypted->algorithm == GP_CIPHER_XOR) {
        uint8_t* result = malloc(encrypted->length);
        if (!result) return NULL;
        
        for (size_t i = 0; i < encrypted->length; i++) {
            result[i] = encrypted->data[i] ^ key[i % key_length];
        }
        
        *output_length = encrypted->length;
        return result;
    }
    
    return NULL;
}

void gp_crypto_encryption_destroy(GPEncryptionResult* result) {
    if (!result) return;
    free(result->data);
    free(result->iv);
    result->data = NULL;
    result->iv = NULL;
    result->length = 0;
    result->iv_length = 0;
}

// Password hashing (simplified)
char* gp_crypto_hash_password(const char* password, const char* salt) {
    if (!password) return NULL;
    
    // Simple password hashing (not secure for production)
    size_t pass_len = strlen(password);
    size_t salt_len = salt ? strlen(salt) : 0;
    size_t total_len = pass_len + salt_len;
    
    uint8_t* combined = malloc(total_len);
    if (!combined) return NULL;
    
    memcpy(combined, password, pass_len);
    if (salt) {
        memcpy(combined + pass_len, salt, salt_len);
    }
    
    GPHashResult hash = gp_crypto_hash(combined, total_len, GP_HASH_SHA256);
    free(combined);
    
    char* hex_hash = gp_crypto_hash_to_hex(&hash);
    gp_crypto_hash_destroy(&hash);
    
    return hex_hash;
}

bool gp_crypto_verify_password(const char* password, const char* hash) {
    if (!password || !hash) return false;
    
    // Extract salt from hash (simplified)
    char* computed_hash = gp_crypto_hash_password(password, "default_salt");
    if (!computed_hash) return false;
    
    bool result = strcmp(computed_hash, hash) == 0;
    free(computed_hash);
    return result;
}

char* gp_crypto_generate_salt(void) {
    return gp_crypto_random_hex(32);
}

// Base64 encoding
static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* gp_crypto_base64_encode(const uint8_t* data, size_t length) {
    if (!data) return NULL;
    
    size_t output_length = 4 * ((length + 2) / 3);
    char* encoded = malloc(output_length + 1);
    if (!encoded) return NULL;
    
    size_t i, j;
    for (i = 0, j = 0; i < length;) {
        uint32_t octet_a = i < length ? data[i++] : 0;
        uint32_t octet_b = i < length ? data[i++] : 0;
        uint32_t octet_c = i < length ? data[i++] : 0;
        
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        
        encoded[j++] = base64_chars[(triple >> 3 * 6) & 0x3F];
        encoded[j++] = base64_chars[(triple >> 2 * 6) & 0x3F];
        encoded[j++] = base64_chars[(triple >> 1 * 6) & 0x3F];
        encoded[j++] = base64_chars[(triple >> 0 * 6) & 0x3F];
    }
    
    // Add padding
    for (i = 0; i < (3 - length % 3) % 3; i++) {
        encoded[output_length - 1 - i] = '=';
    }
    
    encoded[output_length] = '\0';
    return encoded;
}

// Hex encoding
char* gp_crypto_hex_encode(const uint8_t* data, size_t length) {
    if (!data) return NULL;
    
    char* hex = malloc(length * 2 + 1);
    if (!hex) return NULL;
    
    for (size_t i = 0; i < length; i++) {
        sprintf(hex + i * 2, "%02x", data[i]);
    }
    hex[length * 2] = '\0';
    return hex;
}

// Secure memory operations
void gp_crypto_secure_zero(void* ptr, size_t size) {
    if (!ptr) return;
    volatile uint8_t* p = (volatile uint8_t*)ptr;
    for (size_t i = 0; i < size; i++) {
        p[i] = 0;
    }
}

int gp_crypto_secure_compare(const void* a, const void* b, size_t length) {
    if (!a || !b) return -1;
    
    const uint8_t* pa = (const uint8_t*)a;
    const uint8_t* pb = (const uint8_t*)b;
    uint8_t result = 0;
    
    for (size_t i = 0; i < length; i++) {
        result |= pa[i] ^ pb[i];
    }
    
    return result;
}

// UUID generation
char* gp_crypto_uuid_v4(void) {
    uint8_t bytes[16];
    gp_crypto_random_bytes(bytes, 16);
    
    // Set version (4) and variant bits
    bytes[6] = (bytes[6] & 0x0F) | 0x40;
    bytes[8] = (bytes[8] & 0x3F) | 0x80;
    
    char* uuid = malloc(37);
    if (!uuid) return NULL;
    
    sprintf(uuid, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            bytes[0], bytes[1], bytes[2], bytes[3],
            bytes[4], bytes[5], bytes[6], bytes[7],
            bytes[8], bytes[9], bytes[10], bytes[11],
            bytes[12], bytes[13], bytes[14], bytes[15]);
    
    return uuid;
}

// Utility functions
uint64_t gp_crypto_get_timestamp(void) {
    return (uint64_t)time(NULL);
}

bool gp_crypto_constant_time_equal(const uint8_t* a, const uint8_t* b, size_t length) {
    return gp_crypto_secure_compare(a, b, length) == 0;
}

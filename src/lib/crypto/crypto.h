#ifndef GPLANG_CRYPTO_H
#define GPLANG_CRYPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Hash algorithms
typedef enum {
    GP_HASH_MD5,
    GP_HASH_SHA1,
    GP_HASH_SHA256,
    GP_HASH_SHA512,
    GP_HASH_BLAKE2B,
    GP_HASH_CRC32
} GPHashAlgorithm;

// Encryption algorithms
typedef enum {
    GP_CIPHER_AES128,
    GP_CIPHER_AES192,
    GP_CIPHER_AES256,
    GP_CIPHER_CHACHA20,
    GP_CIPHER_XOR,
    GP_CIPHER_RC4
} GPCipherAlgorithm;

// Cipher modes
typedef enum {
    GP_MODE_ECB,
    GP_MODE_CBC,
    GP_MODE_CFB,
    GP_MODE_OFB,
    GP_MODE_CTR,
    GP_MODE_GCM
} GPCipherMode;

// Key derivation functions
typedef enum {
    GP_KDF_PBKDF2,
    GP_KDF_SCRYPT,
    GP_KDF_ARGON2,
    GP_KDF_HKDF
} GPKeyDerivationFunction;

// Digital signature algorithms
typedef enum {
    GP_SIG_RSA,
    GP_SIG_ECDSA,
    GP_SIG_ED25519,
    GP_SIG_DSA
} GPSignatureAlgorithm;

// Hash result structure
typedef struct {
    uint8_t* data;
    size_t length;
    GPHashAlgorithm algorithm;
} GPHashResult;

// Encryption result structure
typedef struct {
    uint8_t* data;
    size_t length;
    uint8_t* iv;
    size_t iv_length;
    GPCipherAlgorithm algorithm;
    GPCipherMode mode;
} GPEncryptionResult;

// Key pair structure
typedef struct {
    uint8_t* public_key;
    size_t public_key_length;
    uint8_t* private_key;
    size_t private_key_length;
    GPSignatureAlgorithm algorithm;
} GPKeyPair;

// Digital signature structure
typedef struct {
    uint8_t* signature;
    size_t length;
    GPSignatureAlgorithm algorithm;
} GPSignature;

// Random number generation
void gp_crypto_init(void);
void gp_crypto_cleanup(void);
void gp_crypto_seed(uint64_t seed);
uint32_t gp_crypto_random_uint32(void);
uint64_t gp_crypto_random_uint64(void);
void gp_crypto_random_bytes(uint8_t* buffer, size_t length);
char* gp_crypto_random_string(size_t length);
char* gp_crypto_random_hex(size_t length);

// Hash functions
GPHashResult gp_crypto_hash(const uint8_t* data, size_t length, GPHashAlgorithm algorithm);
GPHashResult gp_crypto_hash_string(const char* str, GPHashAlgorithm algorithm);
GPHashResult gp_crypto_hash_file(const char* filename, GPHashAlgorithm algorithm);
char* gp_crypto_hash_to_hex(const GPHashResult* hash);
bool gp_crypto_hash_verify(const uint8_t* data, size_t length, 
                          const GPHashResult* expected_hash);
void gp_crypto_hash_destroy(GPHashResult* hash);

// HMAC (Hash-based Message Authentication Code)
GPHashResult gp_crypto_hmac(const uint8_t* key, size_t key_length,
                           const uint8_t* data, size_t data_length,
                           GPHashAlgorithm algorithm);
bool gp_crypto_hmac_verify(const uint8_t* key, size_t key_length,
                          const uint8_t* data, size_t data_length,
                          const GPHashResult* expected_hmac);

// Symmetric encryption/decryption
GPEncryptionResult gp_crypto_encrypt(const uint8_t* data, size_t length,
                                    const uint8_t* key, size_t key_length,
                                    GPCipherAlgorithm algorithm, GPCipherMode mode);
uint8_t* gp_crypto_decrypt(const GPEncryptionResult* encrypted,
                          const uint8_t* key, size_t key_length,
                          size_t* output_length);
void gp_crypto_encryption_destroy(GPEncryptionResult* result);

// Key derivation
uint8_t* gp_crypto_derive_key(const char* password, const uint8_t* salt, size_t salt_length,
                             size_t key_length, int iterations, GPKeyDerivationFunction kdf);

// Password hashing and verification
char* gp_crypto_hash_password(const char* password, const char* salt);
bool gp_crypto_verify_password(const char* password, const char* hash);
char* gp_crypto_generate_salt(void);

// Digital signatures
GPKeyPair gp_crypto_generate_keypair(GPSignatureAlgorithm algorithm, int key_size);
GPSignature gp_crypto_sign(const uint8_t* data, size_t length, 
                          const GPKeyPair* keypair);
bool gp_crypto_verify_signature(const uint8_t* data, size_t length,
                               const GPSignature* signature,
                               const uint8_t* public_key, size_t public_key_length);
void gp_crypto_keypair_destroy(GPKeyPair* keypair);
void gp_crypto_signature_destroy(GPSignature* signature);

// Certificate handling (basic)
typedef struct {
    char* subject;
    char* issuer;
    char* serial_number;
    time_t not_before;
    time_t not_after;
    uint8_t* public_key;
    size_t public_key_length;
} GPCertificate;

GPCertificate* gp_crypto_load_certificate(const char* filename);
bool gp_crypto_verify_certificate(const GPCertificate* cert, const GPCertificate* ca_cert);
void gp_crypto_certificate_destroy(GPCertificate* cert);

// Base64 encoding/decoding
char* gp_crypto_base64_encode(const uint8_t* data, size_t length);
uint8_t* gp_crypto_base64_decode(const char* encoded, size_t* output_length);

// Hex encoding/decoding
char* gp_crypto_hex_encode(const uint8_t* data, size_t length);
uint8_t* gp_crypto_hex_decode(const char* hex, size_t* output_length);

// URL-safe encoding
char* gp_crypto_url_safe_encode(const uint8_t* data, size_t length);
uint8_t* gp_crypto_url_safe_decode(const char* encoded, size_t* output_length);

// Secure memory operations
void* gp_crypto_secure_malloc(size_t size);
void gp_crypto_secure_free(void* ptr, size_t size);
void gp_crypto_secure_zero(void* ptr, size_t size);
int gp_crypto_secure_compare(const void* a, const void* b, size_t length);

// Time-based One-Time Password (TOTP)
char* gp_crypto_totp_generate(const char* secret, uint64_t timestamp, int digits);
bool gp_crypto_totp_verify(const char* token, const char* secret, 
                          uint64_t timestamp, int window);

// UUID generation
char* gp_crypto_uuid_v4(void);
char* gp_crypto_uuid_v5(const char* namespace_uuid, const char* name);

// Compression (for encrypted data)
typedef struct {
    uint8_t* data;
    size_t compressed_size;
    size_t original_size;
} GPCompressedData;

GPCompressedData gp_crypto_compress(const uint8_t* data, size_t length);
uint8_t* gp_crypto_decompress(const GPCompressedData* compressed, size_t* output_length);
void gp_crypto_compressed_destroy(GPCompressedData* compressed);

// Steganography (basic)
uint8_t* gp_crypto_hide_data_in_image(const uint8_t* image_data, size_t image_size,
                                     const uint8_t* secret_data, size_t secret_size,
                                     size_t* output_size);
uint8_t* gp_crypto_extract_data_from_image(const uint8_t* image_data, size_t image_size,
                                          size_t* output_size);

// Blockchain utilities
typedef struct {
    char* previous_hash;
    char* merkle_root;
    uint64_t timestamp;
    uint32_t nonce;
    uint8_t* data;
    size_t data_length;
} GPBlock;

char* gp_crypto_calculate_block_hash(const GPBlock* block);
bool gp_crypto_validate_block(const GPBlock* block, const char* previous_hash);
uint32_t gp_crypto_mine_block(GPBlock* block, int difficulty);

// Zero-knowledge proofs (simplified)
typedef struct {
    uint8_t* proof;
    size_t proof_length;
    uint8_t* public_inputs;
    size_t public_inputs_length;
} GPZKProof;

GPZKProof gp_crypto_generate_zk_proof(const uint8_t* secret, size_t secret_length,
                                      const uint8_t* public_input, size_t public_input_length);
bool gp_crypto_verify_zk_proof(const GPZKProof* proof);
void gp_crypto_zk_proof_destroy(GPZKProof* proof);

// Homomorphic encryption (basic)
typedef struct {
    uint8_t* ciphertext;
    size_t length;
    uint8_t* public_key;
    size_t public_key_length;
} GPHomomorphicCiphertext;

GPHomomorphicCiphertext gp_crypto_homomorphic_encrypt(uint64_t plaintext, 
                                                     const uint8_t* public_key, 
                                                     size_t public_key_length);
GPHomomorphicCiphertext gp_crypto_homomorphic_add(const GPHomomorphicCiphertext* a,
                                                  const GPHomomorphicCiphertext* b);
uint64_t gp_crypto_homomorphic_decrypt(const GPHomomorphicCiphertext* ciphertext,
                                      const uint8_t* private_key, 
                                      size_t private_key_length);

// Utility functions
bool gp_crypto_constant_time_equal(const uint8_t* a, const uint8_t* b, size_t length);
void gp_crypto_wipe_memory(void* ptr, size_t size);
uint64_t gp_crypto_get_timestamp(void);
char* gp_crypto_format_fingerprint(const uint8_t* data, size_t length);

#endif // GPLANG_CRYPTO_H

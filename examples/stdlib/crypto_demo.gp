# GPLANG Crypto Module Demo
# Demonstrates cryptographic and security operations

import crypto

func main():
    print("🔐 GPLANG Crypto Module Demo")
    print("============================")
    print("")
    
    # Initialize crypto subsystem
    crypto.init()
    crypto.seed(12345)  # Seed for reproducible demo
    
    # Random number generation
    print("🎲 Cryptographically Secure Random Generation:")
    print("Random uint32: " + str(crypto.random_uint32()))
    print("Random uint64: " + str(crypto.random_uint64()))
    
    var random_bytes = crypto.random_bytes(16)
    print("Random bytes (16): " + str(random_bytes))
    
    var random_string = crypto.random_string(12)
    print("Random string (12): '" + random_string + "'")
    
    var random_hex = crypto.random_hex(16)
    print("Random hex (16): '" + random_hex + "'")
    print("")
    
    # Hash functions
    print("🔢 Hash Functions:")
    var data = "Hello, GPLANG Crypto!"
    print("Data: '" + data + "'")
    
    var md5_hash = crypto.hash_string(data, crypto.HASH_MD5)
    var sha256_hash = crypto.hash_string(data, crypto.HASH_SHA256)
    var crc32_hash = crypto.hash_string(data, crypto.HASH_CRC32)
    
    print("MD5: " + crypto.hash_to_hex(md5_hash))
    print("SHA256: " + crypto.hash_to_hex(sha256_hash))
    print("CRC32: " + crypto.hash_to_hex(crc32_hash))
    print("")
    
    # HMAC (Hash-based Message Authentication Code)
    print("🔑 HMAC (Message Authentication):")
    var key = "secret_key"
    var message = "Important message"
    
    print("Key: '" + key + "'")
    print("Message: '" + message + "'")
    
    var hmac = crypto.hmac(key, message, crypto.HASH_SHA256)
    print("HMAC-SHA256: " + crypto.hash_to_hex(hmac))
    
    var is_valid = crypto.hmac_verify(key, message, hmac)
    print("HMAC verification: " + str(is_valid))
    print("")
    
    # Symmetric encryption/decryption
    print("🔒 Symmetric Encryption:")
    var plaintext = "This is a secret message!"
    var encryption_key = "my_secret_key_32_bytes_long_123"
    
    print("Plaintext: '" + plaintext + "'")
    print("Key: '" + encryption_key + "'")
    
    var encrypted = crypto.encrypt(plaintext, encryption_key, crypto.CIPHER_XOR, crypto.MODE_ECB)
    print("Encrypted (hex): " + crypto.hex_encode(encrypted.data))
    
    var decrypted = crypto.decrypt(encrypted, encryption_key)
    print("Decrypted: '" + decrypted + "'")
    print("")
    
    # Password hashing and verification
    print("🔐 Password Hashing:")
    var password = "my_secure_password"
    var salt = crypto.generate_salt()
    
    print("Password: '" + password + "'")
    print("Salt: '" + salt + "'")
    
    var hashed_password = crypto.hash_password(password, salt)
    print("Hashed: '" + hashed_password + "'")
    
    var password_valid = crypto.verify_password(password, hashed_password)
    var wrong_password_valid = crypto.verify_password("wrong_password", hashed_password)
    
    print("Correct password valid: " + str(password_valid))
    print("Wrong password valid: " + str(wrong_password_valid))
    print("")
    
    # Base64 encoding/decoding
    print("📝 Base64 Encoding:")
    var original_data = "Hello, World! 🌍"
    print("Original: '" + original_data + "'")
    
    var base64_encoded = crypto.base64_encode(original_data)
    print("Base64 encoded: '" + base64_encoded + "'")
    
    var base64_decoded = crypto.base64_decode(base64_encoded)
    print("Base64 decoded: '" + base64_decoded + "'")
    print("")
    
    # Hex encoding/decoding
    print("🔢 Hex Encoding:")
    var hex_data = "Binary data here"
    print("Original: '" + hex_data + "'")
    
    var hex_encoded = crypto.hex_encode(hex_data)
    print("Hex encoded: '" + hex_encoded + "'")
    
    var hex_decoded = crypto.hex_decode(hex_encoded)
    print("Hex decoded: '" + hex_decoded + "'")
    print("")
    
    # UUID generation
    print("🆔 UUID Generation:")
    var uuid_v4 = crypto.uuid_v4()
    print("UUID v4: " + uuid_v4)
    
    var uuid_v5 = crypto.uuid_v5("6ba7b810-9dad-11d1-80b4-00c04fd430c8", "example.com")
    print("UUID v5: " + uuid_v5)
    print("")
    
    # Time-based One-Time Password (TOTP)
    print("⏰ Time-based OTP (TOTP):")
    var totp_secret = "JBSWY3DPEHPK3PXP"
    var timestamp = crypto.get_timestamp()
    
    print("Secret: '" + totp_secret + "'")
    print("Timestamp: " + str(timestamp))
    
    var totp_token = crypto.totp_generate(totp_secret, timestamp, 6)
    print("TOTP Token: " + totp_token)
    
    var totp_valid = crypto.totp_verify(totp_token, totp_secret, timestamp, 1)
    print("TOTP Valid: " + str(totp_valid))
    print("")
    
    # Digital signatures (simplified demo)
    print("✍️  Digital Signatures:")
    var document = "Important contract document"
    print("Document: '" + document + "'")
    
    # Generate key pair
    var keypair = crypto.generate_keypair(crypto.SIG_RSA, 2048)
    print("Generated RSA-2048 key pair")
    
    # Sign document
    var signature = crypto.sign(document, keypair)
    print("Document signed")
    
    # Verify signature
    var signature_valid = crypto.verify_signature(document, signature, keypair.public_key)
    print("Signature valid: " + str(signature_valid))
    print("")
    
    # Secure memory operations
    print("🛡️  Secure Memory Operations:")
    var sensitive_data = "sensitive_password_123"
    var other_data = "sensitive_password_123"
    var different_data = "different_password_456"
    
    print("Comparing sensitive data securely...")
    var same_result = crypto.secure_compare(sensitive_data, other_data)
    var diff_result = crypto.secure_compare(sensitive_data, different_data)
    
    print("Same data comparison: " + str(same_result == 0))
    print("Different data comparison: " + str(diff_result == 0))
    
    # Securely wipe sensitive data
    crypto.secure_zero(sensitive_data)
    print("Sensitive data securely wiped")
    print("")
    
    # Compression (for encrypted data)
    print("🗜️  Data Compression:")
    var large_data = "This is some data that could benefit from compression. " +
                     "Compression reduces the size of data before encryption. " +
                     "This can improve performance and reduce storage requirements."
    
    print("Original size: " + str(len(large_data)) + " bytes")
    
    var compressed = crypto.compress(large_data)
    print("Compressed size: " + str(compressed.compressed_size) + " bytes")
    print("Compression ratio: " + str(compressed.original_size / compressed.compressed_size) + ":1")
    
    var decompressed = crypto.decompress(compressed)
    print("Decompressed matches original: " + str(decompressed == large_data))
    print("")
    
    # Blockchain utilities
    print("⛓️  Blockchain Utilities:")
    var block = {
        previous_hash: "0000000000000000000000000000000000000000000000000000000000000000",
        merkle_root: "abc123def456",
        timestamp: crypto.get_timestamp(),
        nonce: 0,
        data: "Genesis block data"
    }
    
    print("Mining block with difficulty 4...")
    var nonce = crypto.mine_block(block, 4)
    block.nonce = nonce
    
    var block_hash = crypto.calculate_block_hash(block)
    print("Block hash: " + block_hash)
    print("Block nonce: " + str(nonce))
    
    var block_valid = crypto.validate_block(block, block.previous_hash)
    print("Block valid: " + str(block_valid))
    print("")
    
    # Utility functions
    print("🛠️  Utility Functions:")
    var fingerprint_data = "GPLANG Crypto Module"
    var fingerprint = crypto.format_fingerprint(fingerprint_data)
    print("Fingerprint: " + fingerprint)
    
    var current_timestamp = crypto.get_timestamp()
    print("Current timestamp: " + str(current_timestamp))
    
    var constant_time_equal = crypto.constant_time_equal("abc", "abc")
    print("Constant time equal: " + str(constant_time_equal))
    print("")
    
    # Cleanup
    crypto.cleanup()
    
    print("✨ GPLANG Crypto Module: Enterprise-grade cryptography!")
    print("   Secure hashing, encryption, signatures, and more!")
    print("   🔒 Your data is safe with GPLANG!")
    
    return 0

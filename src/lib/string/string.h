#ifndef GPLANG_STRING_H
#define GPLANG_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdint.h>

// String creation and destruction
char* gp_str_create(const char* str);
char* gp_str_create_empty(size_t capacity);
char* gp_str_create_from_char(char c, size_t count);
void gp_str_destroy(char* str);

// String information
size_t gp_str_length(const char* str);
size_t gp_str_capacity(const char* str);
bool gp_str_is_empty(const char* str);
bool gp_str_is_null_or_empty(const char* str);

// String comparison
int gp_str_compare(const char* str1, const char* str2);
int gp_str_compare_ignore_case(const char* str1, const char* str2);
bool gp_str_equals(const char* str1, const char* str2);
bool gp_str_equals_ignore_case(const char* str1, const char* str2);
bool gp_str_starts_with(const char* str, const char* prefix);
bool gp_str_ends_with(const char* str, const char* suffix);

// String searching
int gp_str_find(const char* str, const char* substr);
int gp_str_find_from(const char* str, const char* substr, size_t start);
int gp_str_find_last(const char* str, const char* substr);
int gp_str_find_char(const char* str, char c);
int gp_str_find_char_from(const char* str, char c, size_t start);
int gp_str_find_char_last(const char* str, char c);
bool gp_str_contains(const char* str, const char* substr);
int gp_str_count(const char* str, const char* substr);

// String modification
char* gp_str_concat(const char* str1, const char* str2);
char* gp_str_concat_many(int count, ...);
char* gp_str_append(char* str, const char* append_str);
char* gp_str_prepend(char* str, const char* prepend_str);
char* gp_str_insert(char* str, size_t index, const char* insert_str);
char* gp_str_remove(char* str, size_t start, size_t length);
char* gp_str_replace(const char* str, const char* old_str, const char* new_str);
char* gp_str_replace_all(const char* str, const char* old_str, const char* new_str);
char* gp_str_replace_char(const char* str, char old_char, char new_char);

// String extraction
char* gp_str_substring(const char* str, size_t start, size_t length);
char* gp_str_left(const char* str, size_t count);
char* gp_str_right(const char* str, size_t count);
char gp_str_char_at(const char* str, size_t index);

// String trimming and padding
char* gp_str_trim(const char* str);
char* gp_str_trim_left(const char* str);
char* gp_str_trim_right(const char* str);
char* gp_str_trim_chars(const char* str, const char* chars);
char* gp_str_pad_left(const char* str, size_t total_width, char pad_char);
char* gp_str_pad_right(const char* str, size_t total_width, char pad_char);
char* gp_str_pad_center(const char* str, size_t total_width, char pad_char);

// String case conversion
char* gp_str_to_upper(const char* str);
char* gp_str_to_lower(const char* str);
char* gp_str_to_title(const char* str);
char* gp_str_capitalize(const char* str);
char* gp_str_swap_case(const char* str);

// String splitting and joining
typedef struct {
    char** parts;
    size_t count;
} StringArray;

StringArray gp_str_split(const char* str, const char* delimiter);
StringArray gp_str_split_char(const char* str, char delimiter);
StringArray gp_str_split_lines(const char* str);
StringArray gp_str_split_words(const char* str);
char* gp_str_join(const StringArray* array, const char* separator);
void gp_str_array_destroy(StringArray* array);

// String formatting
char* gp_str_format(const char* format, ...);
char* gp_str_format_va(const char* format, va_list args);
char* gp_str_repeat(const char* str, size_t count);
char* gp_str_reverse(const char* str);

// String validation and classification
bool gp_str_is_alpha(const char* str);
bool gp_str_is_digit(const char* str);
bool gp_str_is_alnum(const char* str);
bool gp_str_is_space(const char* str);
bool gp_str_is_upper(const char* str);
bool gp_str_is_lower(const char* str);
bool gp_str_is_ascii(const char* str);
bool gp_str_is_printable(const char* str);

// String conversion
int gp_str_to_int(const char* str);
long gp_str_to_long(const char* str);
float gp_str_to_float(const char* str);
double gp_str_to_double(const char* str);
bool gp_str_to_bool(const char* str);
char* gp_int_to_str(int value);
char* gp_long_to_str(long value);
char* gp_float_to_str(float value);
char* gp_double_to_str(double value);
char* gp_bool_to_str(bool value);

// String encoding and escaping
char* gp_str_escape(const char* str);
char* gp_str_unescape(const char* str);
char* gp_str_url_encode(const char* str);
char* gp_str_url_decode(const char* str);
char* gp_str_html_encode(const char* str);
char* gp_str_html_decode(const char* str);
char* gp_str_base64_encode(const char* data, size_t length);
char* gp_str_base64_decode(const char* str, size_t* output_length);

// String hashing and checksums
uint32_t gp_str_hash(const char* str);
uint32_t gp_str_hash_djb2(const char* str);
uint32_t gp_str_hash_fnv1a(const char* str);
uint32_t gp_str_crc32(const char* str);
char* gp_str_md5(const char* str);
char* gp_str_sha1(const char* str);
char* gp_str_sha256(const char* str);

// Pattern matching and regex (simplified)
bool gp_str_match_pattern(const char* str, const char* pattern);
bool gp_str_match_wildcard(const char* str, const char* pattern);
StringArray gp_str_find_all(const char* str, const char* pattern);

// String distance and similarity
int gp_str_levenshtein_distance(const char* str1, const char* str2);
double gp_str_similarity(const char* str1, const char* str2);
double gp_str_jaro_similarity(const char* str1, const char* str2);
double gp_str_jaro_winkler_similarity(const char* str1, const char* str2);

// String compression (simple)
typedef struct {
    char* data;
    size_t compressed_size;
    size_t original_size;
} CompressedString;

CompressedString gp_str_compress(const char* str);
char* gp_str_decompress(const CompressedString* compressed);
void gp_str_compressed_destroy(CompressedString* compressed);

// String builder for efficient concatenation
typedef struct {
    char* buffer;
    size_t length;
    size_t capacity;
} StringBuilder;

StringBuilder* gp_str_builder_create(size_t initial_capacity);
void gp_str_builder_destroy(StringBuilder* builder);
void gp_str_builder_append(StringBuilder* builder, const char* str);
void gp_str_builder_append_char(StringBuilder* builder, char c);
void gp_str_builder_append_format(StringBuilder* builder, const char* format, ...);
void gp_str_builder_clear(StringBuilder* builder);
char* gp_str_builder_to_string(StringBuilder* builder);
size_t gp_str_builder_length(StringBuilder* builder);

// String pool for memory efficiency
typedef struct StringPool StringPool;

StringPool* gp_str_pool_create(void);
void gp_str_pool_destroy(StringPool* pool);
const char* gp_str_pool_intern(StringPool* pool, const char* str);
void gp_str_pool_clear(StringPool* pool);
size_t gp_str_pool_size(StringPool* pool);

// Unicode support (basic)
bool gp_str_is_valid_utf8(const char* str);
size_t gp_str_utf8_length(const char* str);
char* gp_str_utf8_substring(const char* str, size_t start, size_t length);
uint32_t gp_str_utf8_char_at(const char* str, size_t index);

// String templates and interpolation
char* gp_str_template_replace(const char* template_str, const char* key, const char* value);
char* gp_str_interpolate(const char* template_str, ...);

// Utility functions
char* gp_str_random(size_t length);
char* gp_str_random_alpha(size_t length);
char* gp_str_random_alnum(size_t length);
char* gp_str_uuid(void);
char* gp_str_timestamp(void);

#endif // GPLANG_STRING_H

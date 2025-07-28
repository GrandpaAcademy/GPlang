# GPLANG String Module Demo
# Demonstrates advanced string operations

import string

func main():
    print("📝 GPLANG String Module Demo")
    print("============================")
    print("")
    
    # String creation and basic operations
    print("🔤 String Creation & Basic Operations:")
    var text = "Hello, GPLANG World!"
    var empty = string.create_empty(50)
    var repeated = string.create_from_char('*', 10)
    
    print("Original: '" + text + "'")
    print("Length: " + str(string.length(text)))
    print("Is empty: " + str(string.is_empty(text)))
    print("Repeated chars: '" + repeated + "'")
    print("")
    
    # String comparison
    print("⚖️  String Comparison:")
    var str1 = "Apple"
    var str2 = "apple"
    var str3 = "Banana"
    
    print("str1 = '" + str1 + "', str2 = '" + str2 + "', str3 = '" + str3 + "'")
    print("equals(str1, str2): " + str(string.equals(str1, str2)))
    print("equals_ignore_case(str1, str2): " + str(string.equals_ignore_case(str1, str2)))
    print("compare(str1, str3): " + str(string.compare(str1, str3)))
    print("starts_with(text, 'Hello'): " + str(string.starts_with(text, "Hello")))
    print("ends_with(text, 'World!'): " + str(string.ends_with(text, "World!")))
    print("")
    
    # String searching
    print("🔍 String Searching:")
    var search_text = "The quick brown fox jumps over the lazy dog"
    print("Text: '" + search_text + "'")
    print("find('fox'): " + str(string.find(search_text, "fox")))
    print("find('the'): " + str(string.find(search_text, "the")))
    print("find_last('the'): " + str(string.find_last(search_text, "the")))
    print("find_char('o'): " + str(string.find_char(search_text, 'o')))
    print("contains('quick'): " + str(string.contains(search_text, "quick")))
    print("count('o'): " + str(string.count(search_text, "o")))
    print("")
    
    # String modification
    print("✏️  String Modification:")
    var base = "Hello"
    var concatenated = string.concat(base, " World")
    var replaced = string.replace(search_text, "fox", "cat")
    var replaced_all = string.replace_all("banana banana", "na", "XY")
    
    print("concat('Hello', ' World'): '" + concatenated + "'")
    print("replace('fox' -> 'cat'): '" + replaced + "'")
    print("replace_all('na' -> 'XY'): '" + replaced_all + "'")
    print("")
    
    # String extraction
    print("✂️  String Extraction:")
    var sample = "Programming"
    print("Original: '" + sample + "'")
    print("substring(0, 7): '" + string.substring(sample, 0, 7) + "'")
    print("left(4): '" + string.left(sample, 4) + "'")
    print("right(4): '" + string.right(sample, 4) + "'")
    print("char_at(5): '" + string.char_at(sample, 5) + "'")
    print("")
    
    # String trimming and padding
    print("🎨 String Trimming & Padding:")
    var messy = "   Hello World   "
    print("Original: '" + messy + "'")
    print("trim(): '" + string.trim(messy) + "'")
    print("trim_left(): '" + string.trim_left(messy) + "'")
    print("trim_right(): '" + string.trim_right(messy) + "'")
    print("pad_left('Hi', 10, '*'): '" + string.pad_left("Hi", 10, '*') + "'")
    print("pad_right('Hi', 10, '*'): '" + string.pad_right("Hi", 10, '*') + "'")
    print("pad_center('Hi', 10, '*'): '" + string.pad_center("Hi", 10, '*') + "'")
    print("")
    
    # Case conversion
    print("🔄 Case Conversion:")
    var mixed = "Hello World"
    print("Original: '" + mixed + "'")
    print("to_upper(): '" + string.to_upper(mixed) + "'")
    print("to_lower(): '" + string.to_lower(mixed) + "'")
    print("to_title(): '" + string.to_title(mixed) + "'")
    print("capitalize(): '" + string.capitalize(mixed) + "'")
    print("swap_case(): '" + string.swap_case(mixed) + "'")
    print("")
    
    # String splitting and joining
    print("🔗 String Splitting & Joining:")
    var csv = "apple,banana,cherry,date"
    var words = "The quick brown fox"
    
    print("CSV: '" + csv + "'")
    var parts = string.split(csv, ",")
    print("split(','): " + str(parts))
    
    print("Words: '" + words + "'")
    var word_parts = string.split_words(words)
    print("split_words(): " + str(word_parts))
    
    var rejoined = string.join(parts, " | ")
    print("join(' | '): '" + rejoined + "'")
    print("")
    
    # String formatting
    print("📐 String Formatting:")
    var name = "Alice"
    var age = 25
    var formatted = string.format("Hello, %s! You are %d years old.", name, age)
    print("Formatted: '" + formatted + "'")
    
    var repeated_str = string.repeat("Ha", 5)
    print("repeat('Ha', 5): '" + repeated_str + "'")
    
    var reversed = string.reverse("GPLANG")
    print("reverse('GPLANG'): '" + reversed + "'")
    print("")
    
    # String validation
    print("✅ String Validation:")
    var alpha = "Hello"
    var digits = "12345"
    var alnum = "Hello123"
    var spaces = "   "
    
    print("is_alpha('" + alpha + "'): " + str(string.is_alpha(alpha)))
    print("is_digit('" + digits + "'): " + str(string.is_digit(digits)))
    print("is_alnum('" + alnum + "'): " + str(string.is_alnum(alnum)))
    print("is_space('" + spaces + "'): " + str(string.is_space(spaces)))
    print("is_upper('HELLO'): " + str(string.is_upper("HELLO")))
    print("is_lower('hello'): " + str(string.is_lower("hello")))
    print("")
    
    # String conversion
    print("🔄 String Conversion:")
    print("to_int('42'): " + str(string.to_int("42")))
    print("to_float('3.14'): " + str(string.to_float("3.14")))
    print("to_bool('true'): " + str(string.to_bool("true")))
    print("int_to_str(100): '" + string.int_to_str(100) + "'")
    print("float_to_str(2.718): '" + string.float_to_str(2.718) + "'")
    print("bool_to_str(false): '" + string.bool_to_str(false) + "'")
    print("")
    
    # String encoding
    print("🔐 String Encoding:")
    var data = "Hello World"
    var encoded = string.base64_encode(data)
    var decoded = string.base64_decode(encoded)
    
    print("Original: '" + data + "'")
    print("Base64 encoded: '" + encoded + "'")
    print("Base64 decoded: '" + decoded + "'")
    
    var hex_encoded = string.hex_encode(data)
    print("Hex encoded: '" + hex_encoded + "'")
    
    var url_encoded = string.url_encode("Hello World!")
    print("URL encoded: '" + url_encoded + "'")
    print("")
    
    # String hashing
    print("🔢 String Hashing:")
    var hash_text = "GPLANG"
    print("Text: '" + hash_text + "'")
    print("Hash: " + str(string.hash(hash_text)))
    print("DJB2 Hash: " + str(string.hash_djb2(hash_text)))
    print("FNV1a Hash: " + str(string.hash_fnv1a(hash_text)))
    print("CRC32: " + str(string.crc32(hash_text)))
    print("")
    
    # String distance and similarity
    print("📏 String Distance & Similarity:")
    var s1 = "kitten"
    var s2 = "sitting"
    print("String 1: '" + s1 + "'")
    print("String 2: '" + s2 + "'")
    print("Levenshtein distance: " + str(string.levenshtein_distance(s1, s2)))
    print("Similarity: " + str(string.similarity(s1, s2)))
    print("Jaro similarity: " + str(string.jaro_similarity(s1, s2)))
    print("")
    
    # String builder for efficient concatenation
    print("🏗️  String Builder:")
    var builder = string.builder_create(100)
    string.builder_append(builder, "Hello")
    string.builder_append(builder, " ")
    string.builder_append(builder, "World")
    string.builder_append_char(builder, '!')
    string.builder_append_format(builder, " Count: %d", 42)
    
    var built = string.builder_to_string(builder)
    print("Built string: '" + built + "'")
    print("Builder length: " + str(string.builder_length(builder)))
    string.builder_destroy(builder)
    print("")
    
    # Utility functions
    print("🛠️  Utility Functions:")
    var random_str = string.random(10)
    var random_alpha = string.random_alpha(8)
    var random_alnum = string.random_alnum(12)
    var uuid = string.uuid()
    var timestamp = string.timestamp()
    
    print("Random string: '" + random_str + "'")
    print("Random alpha: '" + random_alpha + "'")
    print("Random alnum: '" + random_alnum + "'")
    print("UUID: '" + uuid + "'")
    print("Timestamp: '" + timestamp + "'")
    print("")
    
    print("✨ GPLANG String Module: Powerful string manipulation!")
    print("   Everything you need for text processing and analysis!")
    
    return 0
